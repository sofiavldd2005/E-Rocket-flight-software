# Folder Structure and Directories

```bash
tree L 2 -p src/ trajectory
[drwxr-xr-x]  src/
├── [drwxr-xr-x]  erocket
│   ├── [-rw-r--r--]  CMakeLists.txt
│   ├── [drwxr-xr-x]  config
│   │   └── [-rw-r--r--]  offboard.yaml
│   ├── [drwxr-xr-x]  include
│   │   └── [drwxr-xr-x]  erocket
│   │       ├── [-rw-r--r--]  constants.hpp
│   │       ├── [drwxr-xr-x]  controller
│   │       │   ├── [-rw-r--r--]  allocator.hpp
│   │       │   ├── [drwxr-xr-x]  impls
│   │       │   │   ├── [-rw-r--r--]  attitude_pid.hpp
│   │       │   │   ├── [-rwxr-xr-x]  generic_controller.hpp
│   │       │   │   └── [-rw-r--r--]  position_pid.hpp
│   │       │   ├── [-rw-r--r--]  setpoint.hpp
│   │       │   └── [-rw-r--r--]  state.hpp
│   │       ├── [-rw-r--r--]  emergency_switch.h
│   │       ├── [-rw-r--r--]  frame_transforms.h
│   │       ├── [drwxr-xr-x]  mission
│   │       │   └── [-rw-r--r--]  take_off_and_landing.hpp
│   │       ├── [-rw-r--r--]  setpoints.h
│   │       └── [-rw-r--r--]  vehicle_constants.hpp
│   ├── [drwxr-xr-x]  launch
│   │   ├── [-rw-r--r--]  flight_mode_test.launch.py
│   │   ├── [-rw-r--r--]  mocap_forwarder_test.launch.py
│   │   ├── [-rw-r--r--]  mocap_forwarder.launch.py
│   │   ├── [-rw-r--r--]  offboard_computer_baseline.launch.py
│   │   ├── [-rw-r--r--]  offboard_computer_generic.launch.py
│   │   ├── [-rw-r--r--]  sitl_simulink_baseline.launch.py
│   │   └── [-rw-r--r--]  sitl_simulink_generic.launch.py
│   ├── [drwxr-xr-x]  msg
│   │   ├── [-rw-r--r--]  AllocatorDebug.msg
│   │   ├── [-rw-r--r--]  AttitudeControllerDebug.msg
│   │   ├── [-rw-r--r--]  FlightMode.msg
│   │   ├── [-rw-r--r--]  GenericControllerDebug.msg
│   │   ├── [-rw-r--r--]  PositionControllerDebug.msg
│   │   └── [-rw-r--r--]  SetpointC5.msg
│   ├── [-rw-r--r--]  package.xml
│   ├── [-rw-r--r--]  README.md
│   ├── [drwxr-xr-x]  src
│   │   ├── [-rwxr-xr-x]  baseline_pid_controller.cpp
│   │   ├── [-rwxr-xr-x]  controller_generic.cpp
│   │   ├── [-rw-r--r--]  flight_mode.cpp
│   │   ├── [drwxr-xr-x]  lib
│   │   │   └── [-rw-r--r--]  frame_transforms.cpp
│   │   ├── [-rw-r--r--]  mission.cpp
│   │   └── [-rw-r--r--]  mocap_forwarder.cpp
│   └── [drwxr-xr-x]  test
│       ├── [-rw-r--r--]  __init__.py
│       ├── [drwxr-xr-x]  px4_ros2_communication
│       │   ├── [-rw-r--r--]  flight_mode_test.cpp
│       │   └── [-rw-r--r--]  mocap_forwarder_test.cpp
│       └── [drwxr-xr-x]  sitl
│           └── [-rw-r--r--]  mock_flight_mode.cpp
├── [drwxr-xr-x]  mocap_interface
│   ├── [-rwxr-xr-x]  CMakeLists.txt
│   ├── [drwxr-xr-x]  config
│   │   └── [-rw-r--r--]  tagus.yaml
│   ├── [drwxr-xr-x]  include
│   │   └── [drwxr-xr-x]  mocap_interface
│   │       └── [-rwxr-xr-x]  vrpn_client_ros.hpp
│   ├── [drwxr-xr-x]  launch
│   │   └── [-rwxr-xr-x]  vrpn.launch.py
│   ├── [-rwxr-xr-x]  package.xml
│   └── [drwxr-xr-x]  src
│       └── [-rwxr-xr-x]  vrpn_client_node.cpp
├── [drwxr-xr-x]  px4_msgs
│   ├── [-rw-r--r--]  CMakeLists.txt
│   ├── [-rw-r--r--]  CONTRIBUTING.md
│   ├── [-rw-r--r--]  LICENSE
│   ├── [drwxr-xr-x]  msg
│   ├── [-rw-r--r--]  package.xml
│   ├── [-rw-r--r--]  README.md
│   └── [drwxr-xr-x]  srv
│       └── [-rw-r--r--]  VehicleCommand.srv
├── [drwxr-xr-x]  px4_ros_demos
│   ├── [-rw-r--r--]  CMakeLists.txt
│   ├── [drwxr-xr-x]  launch
│   │   ├── [-rw-r--r--]  actuator_motors.launch.py
│   │   ├── [-rw-r--r--]  actuator_servos.launch.py
│   │   ├── [-rw-r--r--]  offboard_control.launch.py
│   │   └── [-rw-r--r--]  sensor_combined.launch.py
│   ├── [-rw-r--r--]  package.xml
│   ├── [drwxr-xr-x]  px4_ros_demos
│   │   ├── [-rw-r--r--]  __init__.py
│   │   └── [-rw-r--r--]  module_to_import.py
│   ├── [-rw-r--r--]  README.md
│   ├── [drwxr-xr-x]  scripts
│   │   ├── [-rw-r--r--]  __init__.py
│   │   ├── [-rwxr-xr-x]  build_all.bash
│   │   ├── [-rwxr-xr-x]  build_ros2_workspace.bash
│   │   └── [-rwxr-xr-x]  setup_system.bash
│   └── [drwxr-xr-x]  src
│       ├── [-rw-r--r--]  actuator_motors.cpp
│       ├── [-rw-r--r--]  actuator_servos.cpp
│       ├── [-rw-r--r--]  offboard_control.cpp
│       └── [-rw-r--r--]  sensor_combined.cpp
└── [drwxr-xr-x]  vrpn_vendor
    ├── [-rw-r--r--]  CMakeLists.txt
    └── [-rw-r--r--]  package.xml
[drwxr-xr-x]  trajectory
└── [-rw-r--r--]  csv_to_header_with_transform_to_ned.py
```

- the `src/erocket/include/erocket/controller/` contains all math stuff : `attitude_pid.hpp` and `position_pid.hpp`

  - `allocator.hpp`: converts the raw mathematical outputs into physical actuator commands

  - `setpoint.hpp`  and `state.hpp`: Defining the target and the current status.

- `px4_msgs` - official PX4-ROS 2 message definition library (with tweaks?). The nodes use these specific formats (like `VehicleCommand.msg` or `ActuatorServos.msg` )

> [!NOTE]
> See the oficial lib and see what was tweaked

- `src/erocket/msg/` : Pedro created custom messages like `AttitudeControllerDebug.msg` and `AllocatorDebug.msg`. Pedro created a custom packet to transmite the internal mathematical state of his controller. Allows to graph the internal PID math in PlotJuggler in real-time without blocking the main execution thread.

- `src/erocket/test/`: test his mission state machine locally by faking the inputs from PX4.

- `px4_ros_demos`: `actuator_motors.cpp`` and`offboard_control.cp`p are examples provided by the PX4 development team. Prolly Pedro likely used this package as a sandbox to figure out the notoriously tricky PX4-to-ROS 2 DDS bridge before he started architecting the custom erocket package.

> [!NOTE]
> See this demos and try to figure them out be myself.

- `state.hpp`: defines the StateAggregator. Its job is to subscribe to four different high-speed PX4 topics, Auto-Conversion: Every time a new VehicleAttitude arrives, it automatically calculates the rotation_matrix and euler_angles

- `attitude_pid` -> basic PID

- `position_pid` -> Outer Loop of the rocket’s control system

- `src/erocket/launch/sitl_simulink_baseline.launch.py`: This is the launch file that boots up the ROS 2 nodes in "Simulation Mode".

- `src/erocket/test/sitl/mock_flight_mode.cpp`:  Mock node to bypass some of the real hardware checks.
