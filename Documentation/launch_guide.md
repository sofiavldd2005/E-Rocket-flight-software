# E-Rocket Flight Software Launch Guide

This document explains the startup process for the E-Rocket flight software, including the available launch files, the environments they support, and the nodes they execute.

## Prerequisites
Before running any launch files, make sure your ROS 2 workspace is built and sourced:
```bash
cd workspace_erocket
colcon build
source install/setup.bash # or source install/setup.zsh for zsh
```

Additionally, the PX4 flight controller communicates with ROS 2 via the **MicroXRCEAgent**. By default, this is handled externally (e.g., via `mprocs` over a serial port like `/dev/ttyACM0`). Ensure the agent is running before launching the ROS 2 nodes so that the software can communicate with the hardware.

## Launch Files Overview

The `erocket` package provides several launch files depending on the target environment (Physical Hardware vs. SITL Simulation) and the selected controller (Baseline PID vs. Generic).

### 1. Hardware Flight: Baseline PID Controller
**File**: `offboard_computer_baseline.launch.py`  
**Command**: 
```bash
ros2 launch erocket offboard_computer_baseline.launch.py
```
**Purpose**: Starts the core flight stack using the pre-configured Baseline PID controller for actual hardware flights.  
**Nodes Started**:
- `baseline_pid_controller`: Subscribes to IMU/MoCap data, runs the PID math, and allocates TVC actuator commands.
- `flight_mode`: Manages the state machine (Waiting, Armed, Manual, Offboard).
- `mission`: Coordinates the autonomous mission phases (Takeoff, Hover, Land).

### 2. Hardware Flight: Generic Controller
**File**: `offboard_computer_generic.launch.py`  
**Command**: 
```bash
ros2 launch erocket offboard_computer_generic.launch.py
```
**Purpose**: Starts the flight stack using a custom template controller instead of the baseline PID. Ideal for testing new control algorithms.  
**Nodes Started**:
- `controller_generic`: User-defined custom control logic.
- `flight_mode`
- `mission`

### 3. Software in the Loop (SITL) Simulation: Baseline PID
**File**: `sitl_simulink_baseline.launch.py`  
**Command**: 
```bash
ros2 launch erocket sitl_simulink_baseline.launch.py
```
**Purpose**: Used for testing the flight software in a simulation environment without physical hardware.  
**Nodes Started**:
- `baseline_pid_controller`
- `mission`
- `mock_flight_mode`: Replaces the hardware flight mode node to safely mock transition states.
**Overrides**: Safely injects the parameters `{"servo_active": False, "motor_active": False}` to guarantee actuators remain locked out during simulation.

### 4. Software in the Loop (SITL) Simulation: Generic
**File**: `sitl_simulink_generic.launch.py`  
**Command**: 
```bash
ros2 launch erocket sitl_simulink_generic.launch.py
```
**Purpose**: Same as the baseline SITL simulation, but executes the `controller_generic` node for simulation testing.

### 5. Motion Capture System (MoCap) Forwarder
**File**: `mocap_forwarder.launch.py`  
**Command**: 
```bash
ros2 launch erocket mocap_forwarder.launch.py
```
**Purpose**: Connects the ROS 2 software to the VRPN motion capture system (e.g., OptiTrack) and forwards localization data to the flight controller.  
**Nodes Started**:
- `vrpn_client_ros` (via the `mocap_interface` package): Receives live VRPN data.
- `mocap_forwarder`: Translates the MoCap data into PX4-compatible `VehicleOdometry` messages and bridges them over the MicroXRCE network.

## Configuration Parameters

All launch files feed the exact same parameter file to the nodes. The centralized configuration is located at:
`src/erocket/config/offboard.yaml`

This file dictates crucial runtime configurations, including:
- PID Gains (Proportional, Derivative, Integral).
- Actuator constraints (Max PWM, Servo angles).
- Physical vehicle characteristics (Mass, Inertia, Lever Arm).
- Active Controllers toggles.

Ensure this file is properly configured before initiating any launch sequence.
