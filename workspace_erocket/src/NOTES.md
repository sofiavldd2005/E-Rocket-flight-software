# Folder Structure and Directories
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
