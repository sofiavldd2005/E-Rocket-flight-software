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

ROS2 publishes (t=0)
  → DDS serializes message
  → MAVLink or uORB bridge transfers over serial/UDP
  → PX4 receives & processes
  → STM32 PWM output (t=5-50ms depending on loop rate)

---

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
