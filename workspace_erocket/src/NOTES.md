# Folder Structure and Directories

```bash
tree L 2 -p src/ trajectory
L  [error opening dir]
2  [error opening dir]
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
│   │   ├── [-rw-r--r--]  ActionRequest.msg
│   │   ├── [-rw-r--r--]  ActuatorArmed.msg
│   │   ├── [-rw-r--r--]  ActuatorControlsStatus.msg
│   │   ├── [-rw-r--r--]  ActuatorMotors.msg
│   │   ├── [-rw-r--r--]  ActuatorOutputs.msg
│   │   ├── [-rw-r--r--]  ActuatorServos.msg
│   │   ├── [-rw-r--r--]  ActuatorServosTrim.msg
│   │   ├── [-rw-r--r--]  ActuatorTest.msg
│   │   ├── [-rw-r--r--]  AdcReport.msg
│   │   ├── [-rw-r--r--]  Airspeed.msg
│   │   ├── [-rw-r--r--]  AirspeedValidated.msg
│   │   ├── [-rw-r--r--]  AirspeedWind.msg
│   │   ├── [-rw-r--r--]  ArmingCheckReply.msg
│   │   ├── [-rw-r--r--]  ArmingCheckRequest.msg
│   │   ├── [-rw-r--r--]  AutotuneAttitudeControlStatus.msg
│   │   ├── [-rw-r--r--]  BatteryStatus.msg
│   │   ├── [-rw-r--r--]  Buffer128.msg
│   │   ├── [-rw-r--r--]  ButtonEvent.msg
│   │   ├── [-rw-r--r--]  CameraCapture.msg
│   │   ├── [-rw-r--r--]  CameraStatus.msg
│   │   ├── [-rw-r--r--]  CameraTrigger.msg
│   │   ├── [-rw-r--r--]  CanInterfaceStatus.msg
│   │   ├── [-rw-r--r--]  CellularStatus.msg
│   │   ├── [-rw-r--r--]  CollisionConstraints.msg
│   │   ├── [-rw-r--r--]  CollisionReport.msg
│   │   ├── [-rw-r--r--]  ConfigOverrides.msg
│   │   ├── [-rw-r--r--]  ControlAllocatorStatus.msg
│   │   ├── [-rw-r--r--]  Cpuload.msg
│   │   ├── [-rw-r--r--]  DatamanRequest.msg
│   │   ├── [-rw-r--r--]  DatamanResponse.msg
│   │   ├── [-rw-r--r--]  DebugArray.msg
│   │   ├── [-rw-r--r--]  DebugKeyValue.msg
│   │   ├── [-rw-r--r--]  DebugValue.msg
│   │   ├── [-rw-r--r--]  DebugVect.msg
│   │   ├── [-rw-r--r--]  DifferentialDriveSetpoint.msg
│   │   ├── [-rw-r--r--]  DifferentialPressure.msg
│   │   ├── [-rw-r--r--]  DistanceSensor.msg
│   │   ├── [-rw-r--r--]  Ekf2Timestamps.msg
│   │   ├── [-rw-r--r--]  EscReport.msg
│   │   ├── [-rw-r--r--]  EscStatus.msg
│   │   ├── [-rw-r--r--]  EstimatorAidSource1d.msg
│   │   ├── [-rw-r--r--]  EstimatorAidSource2d.msg
│   │   ├── [-rw-r--r--]  EstimatorAidSource3d.msg
│   │   ├── [-rw-r--r--]  EstimatorBias.msg
│   │   ├── [-rw-r--r--]  EstimatorBias3d.msg
│   │   ├── [-rw-r--r--]  EstimatorEventFlags.msg
│   │   ├── [-rw-r--r--]  EstimatorGpsStatus.msg
│   │   ├── [-rw-r--r--]  EstimatorInnovations.msg
│   │   ├── [-rw-r--r--]  EstimatorSelectorStatus.msg
│   │   ├── [-rw-r--r--]  EstimatorSensorBias.msg
│   │   ├── [-rw-r--r--]  EstimatorStates.msg
│   │   ├── [-rw-r--r--]  EstimatorStatus.msg
│   │   ├── [-rw-r--r--]  EstimatorStatusFlags.msg
│   │   ├── [-rw-r--r--]  Event.msg
│   │   ├── [-rw-r--r--]  FailsafeFlags.msg
│   │   ├── [-rw-r--r--]  FailureDetectorStatus.msg
│   │   ├── [-rw-r--r--]  FigureEightStatus.msg
│   │   ├── [-rw-r--r--]  FlightPhaseEstimation.msg
│   │   ├── [-rw-r--r--]  FollowTarget.msg
│   │   ├── [-rw-r--r--]  FollowTargetEstimator.msg
│   │   ├── [-rw-r--r--]  FollowTargetStatus.msg
│   │   ├── [-rw-r--r--]  GeneratorStatus.msg
│   │   ├── [-rw-r--r--]  GeofenceResult.msg
│   │   ├── [-rw-r--r--]  GeofenceStatus.msg
│   │   ├── [-rw-r--r--]  GimbalControls.msg
│   │   ├── [-rw-r--r--]  GimbalDeviceAttitudeStatus.msg
│   │   ├── [-rw-r--r--]  GimbalDeviceInformation.msg
│   │   ├── [-rw-r--r--]  GimbalDeviceSetAttitude.msg
│   │   ├── [-rw-r--r--]  GimbalManagerInformation.msg
│   │   ├── [-rw-r--r--]  GimbalManagerSetAttitude.msg
│   │   ├── [-rw-r--r--]  GimbalManagerSetManualControl.msg
│   │   ├── [-rw-r--r--]  GimbalManagerStatus.msg
│   │   ├── [-rw-r--r--]  GotoSetpoint.msg
│   │   ├── [-rw-r--r--]  GpioConfig.msg
│   │   ├── [-rw-r--r--]  GpioIn.msg
│   │   ├── [-rw-r--r--]  GpioOut.msg
│   │   ├── [-rw-r--r--]  GpioRequest.msg
│   │   ├── [-rw-r--r--]  GpsDump.msg
│   │   ├── [-rw-r--r--]  GpsInjectData.msg
│   │   ├── [-rw-r--r--]  Gripper.msg
│   │   ├── [-rw-r--r--]  HealthReport.msg
│   │   ├── [-rw-r--r--]  HeaterStatus.msg
│   │   ├── [-rw-r--r--]  HomePosition.msg
│   │   ├── [-rw-r--r--]  HoverThrustEstimate.msg
│   │   ├── [-rw-r--r--]  InputRc.msg
│   │   ├── [-rw-r--r--]  InternalCombustionEngineStatus.msg
│   │   ├── [-rw-r--r--]  IridiumsbdStatus.msg
│   │   ├── [-rw-r--r--]  IrlockReport.msg
│   │   ├── [-rw-r--r--]  LandingGear.msg
│   │   ├── [-rw-r--r--]  LandingGearWheel.msg
│   │   ├── [-rw-r--r--]  LandingTargetInnovations.msg
│   │   ├── [-rw-r--r--]  LandingTargetPose.msg
│   │   ├── [-rw-r--r--]  LaunchDetectionStatus.msg
│   │   ├── [-rw-r--r--]  LedControl.msg
│   │   ├── [-rw-r--r--]  LoggerStatus.msg
│   │   ├── [-rw-r--r--]  LogMessage.msg
│   │   ├── [-rw-r--r--]  MagnetometerBiasEstimate.msg
│   │   ├── [-rw-r--r--]  MagWorkerData.msg
│   │   ├── [-rw-r--r--]  ManualControlSetpoint.msg
│   │   ├── [-rw-r--r--]  ManualControlSwitches.msg
│   │   ├── [-rw-r--r--]  MavlinkLog.msg
│   │   ├── [-rw-r--r--]  MavlinkTunnel.msg
│   │   ├── [-rw-r--r--]  MessageFormatRequest.msg
│   │   ├── [-rw-r--r--]  MessageFormatResponse.msg
│   │   ├── [-rw-r--r--]  Mission.msg
│   │   ├── [-rw-r--r--]  MissionResult.msg
│   │   ├── [-rw-r--r--]  ModeCompleted.msg
│   │   ├── [-rw-r--r--]  MountOrientation.msg
│   │   ├── [-rw-r--r--]  NavigatorMissionItem.msg
│   │   ├── [-rw-r--r--]  NormalizedUnsignedSetpoint.msg
│   │   ├── [-rw-r--r--]  NpfgStatus.msg
│   │   ├── [-rw-r--r--]  ObstacleDistance.msg
│   │   ├── [-rw-r--r--]  OffboardControlMode.msg
│   │   ├── [-rw-r--r--]  OnboardComputerStatus.msg
│   │   ├── [-rw-r--r--]  OrbitStatus.msg
│   │   ├── [-rw-r--r--]  OrbTest.msg
│   │   ├── [-rw-r--r--]  OrbTestLarge.msg
│   │   ├── [-rw-r--r--]  OrbTestMedium.msg
│   │   ├── [-rw-r--r--]  ParameterResetRequest.msg
│   │   ├── [-rw-r--r--]  ParameterSetUsedRequest.msg
│   │   ├── [-rw-r--r--]  ParameterSetValueRequest.msg
│   │   ├── [-rw-r--r--]  ParameterSetValueResponse.msg
│   │   ├── [-rw-r--r--]  ParameterUpdate.msg
│   │   ├── [-rw-r--r--]  Ping.msg
│   │   ├── [-rw-r--r--]  PositionControllerLandingStatus.msg
│   │   ├── [-rw-r--r--]  PositionControllerStatus.msg
│   │   ├── [-rw-r--r--]  PositionSetpoint.msg
│   │   ├── [-rw-r--r--]  PositionSetpointTriplet.msg
│   │   ├── [-rw-r--r--]  PowerButtonState.msg
│   │   ├── [-rw-r--r--]  PowerMonitor.msg
│   │   ├── [-rw-r--r--]  PpsCapture.msg
│   │   ├── [-rw-r--r--]  PwmInput.msg
│   │   ├── [-rw-r--r--]  Px4ioStatus.msg
│   │   ├── [-rw-r--r--]  QshellReq.msg
│   │   ├── [-rw-r--r--]  QshellRetval.msg
│   │   ├── [-rw-r--r--]  RadioStatus.msg
│   │   ├── [-rw-r--r--]  RateCtrlStatus.msg
│   │   ├── [-rw-r--r--]  RcChannels.msg
│   │   ├── [-rw-r--r--]  RcParameterMap.msg
│   │   ├── [-rw-r--r--]  RegisterExtComponentReply.msg
│   │   ├── [-rw-r--r--]  RegisterExtComponentRequest.msg
│   │   ├── [-rw-r--r--]  Rpm.msg
│   │   ├── [-rw-r--r--]  RtlStatus.msg
│   │   ├── [-rw-r--r--]  RtlTimeEstimate.msg
│   │   ├── [-rw-r--r--]  SatelliteInfo.msg
│   │   ├── [-rw-r--r--]  SensorAccel.msg
│   │   ├── [-rw-r--r--]  SensorAccelFifo.msg
│   │   ├── [-rw-r--r--]  SensorAirflow.msg
│   │   ├── [-rw-r--r--]  SensorBaro.msg
│   │   ├── [-rw-r--r--]  SensorCombined.msg
│   │   ├── [-rw-r--r--]  SensorCorrection.msg
│   │   ├── [-rw-r--r--]  SensorGnssRelative.msg
│   │   ├── [-rw-r--r--]  SensorGps.msg
│   │   ├── [-rw-r--r--]  SensorGyro.msg
│   │   ├── [-rw-r--r--]  SensorGyroFft.msg
│   │   ├── [-rw-r--r--]  SensorGyroFifo.msg
│   │   ├── [-rw-r--r--]  SensorHygrometer.msg
│   │   ├── [-rw-r--r--]  SensorMag.msg
│   │   ├── [-rw-r--r--]  SensorOpticalFlow.msg
│   │   ├── [-rw-r--r--]  SensorPreflightMag.msg
│   │   ├── [-rw-r--r--]  SensorSelection.msg
│   │   ├── [-rw-r--r--]  SensorsStatus.msg
│   │   ├── [-rw-r--r--]  SensorsStatusImu.msg
│   │   ├── [-rw-r--r--]  SensorUwb.msg
│   │   ├── [-rw-r--r--]  SystemPower.msg
│   │   ├── [-rw-r--r--]  TakeoffStatus.msg
│   │   ├── [-rw-r--r--]  TaskStackInfo.msg
│   │   ├── [-rw-r--r--]  TecsStatus.msg
│   │   ├── [-rw-r--r--]  TelemetryStatus.msg
│   │   ├── [-rw-r--r--]  TiltrotorExtraControls.msg
│   │   ├── [-rw-r--r--]  TimesyncStatus.msg
│   │   ├── [-rw-r--r--]  TrajectoryBezier.msg
│   │   ├── [-rw-r--r--]  TrajectorySetpoint.msg
│   │   ├── [-rw-r--r--]  TrajectoryWaypoint.msg
│   │   ├── [-rw-r--r--]  TransponderReport.msg
│   │   ├── [-rw-r--r--]  TuneControl.msg
│   │   ├── [-rw-r--r--]  UavcanParameterRequest.msg
│   │   ├── [-rw-r--r--]  UavcanParameterValue.msg
│   │   ├── [-rw-r--r--]  UlogStream.msg
│   │   ├── [-rw-r--r--]  UlogStreamAck.msg
│   │   ├── [-rw-r--r--]  UnregisterExtComponent.msg
│   │   ├── [-rw-r--r--]  VehicleAcceleration.msg
│   │   ├── [-rw-r--r--]  VehicleAirData.msg
│   │   ├── [-rw-r--r--]  VehicleAngularAccelerationSetpoint.msg
│   │   ├── [-rw-r--r--]  VehicleAngularVelocity.msg
│   │   ├── [-rw-r--r--]  VehicleAttitude.msg
│   │   ├── [-rw-r--r--]  VehicleAttitudeSetpoint.msg
│   │   ├── [-rw-r--r--]  VehicleCommand.msg
│   │   ├── [-rw-r--r--]  VehicleCommandAck.msg
│   │   ├── [-rw-r--r--]  VehicleConstraints.msg
│   │   ├── [-rw-r--r--]  VehicleControlMode.msg
│   │   ├── [-rw-r--r--]  VehicleGlobalPosition.msg
│   │   ├── [-rw-r--r--]  VehicleImu.msg
│   │   ├── [-rw-r--r--]  VehicleImuStatus.msg
│   │   ├── [-rw-r--r--]  VehicleLandDetected.msg
│   │   ├── [-rw-r--r--]  VehicleLocalPosition.msg
│   │   ├── [-rw-r--r--]  VehicleLocalPositionSetpoint.msg
│   │   ├── [-rw-r--r--]  VehicleMagnetometer.msg
│   │   ├── [-rw-r--r--]  VehicleOdometry.msg
│   │   ├── [-rw-r--r--]  VehicleOpticalFlow.msg
│   │   ├── [-rw-r--r--]  VehicleOpticalFlowVel.msg
│   │   ├── [-rw-r--r--]  VehicleRatesSetpoint.msg
│   │   ├── [-rw-r--r--]  VehicleRoi.msg
│   │   ├── [-rw-r--r--]  VehicleStatus.msg
│   │   ├── [-rw-r--r--]  VehicleThrustSetpoint.msg
│   │   ├── [-rw-r--r--]  VehicleTorqueSetpoint.msg
│   │   ├── [-rw-r--r--]  VehicleTrajectoryBezier.msg
│   │   ├── [-rw-r--r--]  VehicleTrajectoryWaypoint.msg
│   │   ├── [-rw-r--r--]  VelocityLimits.msg
│   │   ├── [-rw-r--r--]  VtolVehicleStatus.msg
│   │   ├── [-rw-r--r--]  WheelEncoders.msg
│   │   ├── [-rw-r--r--]  Wind.msg
│   │   └── [-rw-r--r--]  YawEstimatorStatus.msg
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

31 directories, 284 files
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

## Update about mprocs.yaml

mprocs.yaml orchestrator controls the rocket.

Pedro registered a parameter_callback() function using add_on_set_parameters_callback. This means the node is actively listening to the ROS 2 network for configuration changes.
When you click the START_MISSION button in mprocs, it runs:
ros2 param set /mission offboard.flight_mode 4

The parameter callback inside mission.cpp intercepts that 4, translates it to FlightMode::IN_MISSION, and calls request_flight_mode(new_flight_mode). The node then publishes a flight mode switch message, officially transitioning the rocket into the active mission state.
