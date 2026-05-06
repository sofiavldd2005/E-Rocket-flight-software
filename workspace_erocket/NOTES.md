# Folder Structure

```bash
tree -L 1
.
├── build
├── CONTROLLER.xml
├── install
├── log
├── mprocs.yaml
├── PAPER - TEST 1
├── PAPER - TEST 2 - fast trajectory, full battery
├── PAPER - TEST 3 - fast trajectory, full battery
├── rosbags
├── src
├── todo.md
└── trajectory

10 directories, 3 files

```

- `build`, `install`, `log`, and `src` -> Stuff to use with the standard `colcon` build system.

- `rosbags` -> mcap instead of SQLite DB?

> [!NOTE]
> Investigate this

- `mcprocs.yaml` -> Instead of start the DDS bridge, and the ROS nodes manualy, Pedro is using mprocs (a tmux) to launch the entire vehicle architecture at once.

> [!NOTE]
> Pedro is using bash, i don't like bash, i use zsh, change all the source cms to `.zsh`, if I actualy change to zsh.
> hardcoded `tty` ports, maybe have a script to find out which ones are being used, maybe adapt the script i have for debugging the STM, i figure the ports might change over time.

- maps out the software architecture of the E-Rocket: how it talks to the hardware, and how he controls the state machine.
- The Hardware Bridges (Talking to PX4)
  - MicroXRCEAgent the uXRCE-DDS bridge, serial connection (/dev/ttyACM0 at 921600 baud) directly to the PX4 hardware.
  - This is the pipeline where your Topics   (like TVC angles and IMU data) flow back and forth.
- MAVLINK_Forwarder: This uses mavproxy.py to grab standard MAVLink telemetry from a serial port (ttyAMA10) and send it over Wi-Fi (udpbcast:192.168.1.255). This is how QGroundControl (the standard drone ground station software) monitors the rocket.

- system into two different modes: Physical Hardware and Simulation.

  - Hardware Mode: Offboard_Baseline and Offboard_Generic run the real launch files (e.g., offboard_computer_baseline.launch.py).

  - Simulation Mode: SITL_SIMULINK_Baseline runs a Software-In-The-Loop (SITL) simulation. Test the  ROS 2 code against a Simulink physics model of the rocket.

- The State Machine (Mission Control)

```bash
ros2 param set /mission offboard.flight_mode 3

```

- He is using Parameters to control the flight state machine, There is a node running called /mission. Instead of using Actions or Services, he transitions the rocket between states (Mode 3, Mode 4, Mode 5) by updating a parameter live over the network.

- Data & Mocap
  - MOCAP: This boots up mocap_forwarder.launch.py, which is the node we that likely uses TF2 to convert the VRPN camera data into rocket coordinates.
  - ROS_Bag & Plot_Juggler: yo record every single topic (ros2 bag record -a) and autostarts PlotJuggler for telemetry viewing.

- `trajectory`  $\rightarrow$  Python script that turns a CSV with an trajectory to a `.h` file.

- `CONTROLLER.xml`  $\rightarrow$  pre-configured layout file for [PlotJuggler](https://plotjuggler.com/)?

## Flow

Sensors $\rightarrow$ baseline_pid_controller.cpp $\rightarrow$ allocator.hpp $\rightarrow$ PX4 Hardware

---

## Complaints from Gemini - TODO actualy read the complaints and if they make sense

1. The static Time Trap (Massive Bug)
In baseline_pid_controller.cpp, inside the FlightMode::ARM state, Pedro wrote this to trigger the servo wiggle:

C++
static rclcpp::Time t0 = this->get_clock()->now();
This is a critical bug. Because t0 is declared static, it is only initialized the very first time the system enters the ARM state. If the rocket drops out of ARM and goes back in, t0 does not reset. The logic now - t0 < 3.0s will instantly fail, the rocket won't wiggle, and it will immediately jump to the now - t0 > 4.0s condition. Relying on static local variables inside a state machine loop is a huge red flag.

1. Blocking the ROS 2 Executor
Look at the constructor of Allocator in allocator.hpp. He wrote a while loop that forces the node to sleep for a full second:

```Cpp
auto t0 = node->get_clock()->now();
while (node->get_clock()->now() - t0 < 1s) {
    publish_servo_pwm();
    publish_motor_pwm();
    rclcpp::sleep_for(100ms);
}
```

This is a major ROS 2 anti-pattern. Constructors run on the main thread. By putting rclcpp::sleep_for(100ms) in the constructor, he is entirely blocking the ROS executor from spinning. No callbacks can be processed, and no topic subscriptions will update during this time. This should have been handled by a startup timer or a lifecycle node state transition, not a blocking while-loop.

1. Unsafe Math / NaN Propagation
In allocator.hpp, the physical translation logic is completely missing mathematical safeguards:

```Cpp
gamma_inner_ = std::asin(input.thrust_vector[1] / thrust_);
```

If thrust_is exactly 0.0, that is a division by zero. Furthermore, if numerical noise causes input.thrust_vector[1] / thrust_to be 1.000001, std::asin will return NaN. Because the limit_range_servo_pwm function just checks if (servo_pwm > 1.0f), a NaN value will silently bypass the bounds check and get sent straight to the hardware. In Rust, the compiler or a strict Result/Option pattern would have forced him to handle these edge cases. Here, it's just hoping the math stays clean.

1. Dirty Shutdowns
In baseline_pid_controller.cpp, when the state is FlightMode::ABORT, he calls rclcpp::shutdown(); directly inside the timer callback. This is generally considered terrible practice. It abruptly kills the node from inside a worker thread, which can cause exceptions in the executor or leave network sockets hanging. It's much cleaner to publish a zeroed-out safe state, set a flag, and let the main executor gracefully tear down.

## Here are the most glaring issues in mission.cpp

1. The "Infinite Loop of Death" (Catastrophic Bug)
Look at what happens in the parameter callback when a user tries to trigger an abort from the ground station:

```Cpp
else if (param.get_name() == FLIGHT_MODE_PARAM) {
    uint8_t new_flight_mode = param.as_int();
    RCLCPP_INFO(this->get_logger(), "Requesting flight mode to: %d", new_flight_mode);
    request_flight_mode(new_flight_mode);

    while (new_flight_mode == FlightMode::ABORT) {
        request_flight_mode(FlightMode::ABORT);
    }
}

```

If new_flight_mode equals FlightMode::ABORT, it enters a while loop. But request_flight_mode() just publishes a ROS message; it never modifies the new_flight_mode variable.
This is an infinite loop. The moment you abort the rocket via a parameter change, the entire mission node will completely hang, freezing the parameter callback thread forever and blasting the network with millions of abort messages until the computer crashes.

1. The "Static Variable" Epidemic (Single-Use Rocket)
We saw this bug in the PID controller, but in mission.cpp, it is everywhere. Pedro has built a state machine that can strictly only be run once.

Look at FlightMode::TAKE_OFF:

```Cpp
static rclcpp::Time takeoff_start_time = this->get_clock()->now();
Look at FlightMode::LANDING:

```

```Cpp
static State initial_landing_state = state_aggregator_->get_state();
static rclcpp::Time landing_start_time = this->get_clock()->now();
```

If you arm the rocket, start taking off, realize something is wrong, land, and try to take off again, the rocket will crash. Because these variables are static, they do not reset when you re-enter the state. The math for compute_takeoff() will think the rocket has been taking off for 45 minutes, resulting in a massive, violent setpoint jump that will rip the motors apart.
A real state machine requires explicit entry and exit transition functions to cleanly reset internal timers and states.

1. Bizarre C++ Syntax (The `#include` placement)
Look inside the FlightMode::IN_MISSION block in the mission() callback:

```Cpp
if (trajectory_setpoint_active_) {
    #include "setpoints.h"
    static uint32_t index = 0;
    // ...
```

He put an `#include` directive inside an if statement, in the middle of a function body.
While the C++ preprocessor will technically allow this (it just pastes the raw text of the header file right there before compiling), it is a massive anti-pattern. If setpoints.h contains anything other than a simple anonymous array, it will cause scope nightmares. Plus, because of the static uint32_t index = 0; immediately below it, if you ever try to run the trajectory twice, it won't start from the beginning.

1. Asynchronous Aborts (Unsafe Design)
Look at how the emergency switch triggers an abort:

```Cpp
if (emergency_switch_.emergency_switch_on()) {
    if (flight_mode_.load() != FlightMode::ABORT) {
        RCLCPP_ERROR(this->get_logger(), "Emergency switch activated! Aborting mission...");
        request_flight_mode(FlightMode::ABORT);
    }
    return;
}

```

When the emergency switch is hit, it calls request_flight_mode(FlightMode::ABORT). As we know, that function merely publishes a ROS message to the FLIGHT_MODE_SET_TOPIC.
This means the "Kill Switch" relies on the DDS network stack. If the network is congested, or if the flight_mode_get_subscriber_ callback is delayed, the rocket will keep flying. A true emergency kill switch should directly and synchronously alter the state memory, bypass the network, or cut hardware power.
