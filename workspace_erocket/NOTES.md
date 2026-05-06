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

- `trajectory` -> Python script that turns a CSV with an trajectory to a `.h` file.

- `CONTROLLER.xml` -> pre-configured layout file for [PlotJuggler](https://plotjuggler.com/)?
