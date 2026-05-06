"""
Launch a offboard computer with pid controller.
"""

from launch import LaunchDescription
from launch.actions import ExecuteProcess
from launch_ros.actions import Node
import os

# all nodes are fed the exact same parameter file: PID gains or setpoint`s
launch_file_dir = os.path.dirname(os.path.realpath(__file__))
config_file = os.path.realpath(
    os.path.join(launch_file_dir, "../../../../../src/erocket/config/offboard.yaml")
)


def generate_launch_description():
    # commented out in the return statement #[micro_ros_agent].
    # This is because in the mprocs.yaml, he runs MicroXRCEAgent over a serial port (/dev/ttyACM0) instead
    micro_ros_agent = ExecuteProcess(
        cmd=[["micro-ros-agent udp4 --port 8888 -v "]], shell=True
    )
    # code that lives in node src/erocket/src/baseline_pid_controller.cpp
    # subscribes to the VRPN mocap data and the PX4 IMU data, runs the PID math, and publishes TVC actuator commands back to PX4?
    baseline_pid_controller_node = Node(
        package="erocket",
        executable="baseline_pid_controller",
        output="screen",
        shell=True,
        parameters=[config_file],
    )
    # node that handles transition states (e.g., waiting for arming, armed, manual mode, offboard mode).
    flight_mode_node = Node(
        package="erocket",
        executable="flight_mode",
        output="screen",
        shell=True,
        arguments=["--ros-args", "--log-level", "warn"],
        parameters=[config_file],
    )
    # saw this in the mprocs.yaml - listens to the Parameters (ros2 param set /mission offboard.flight_mode 3)
    # and coordinates the mission phases (Lift Off, Start Mission, Land).
    mission_node = Node(
        package="erocket",
        executable="mission",
        output="screen",
        shell=True,
        parameters=[config_file],
    )

    return LaunchDescription(
        [
            # micro_ros_agent,
            baseline_pid_controller_node,
            flight_mode_node,
            mission_node,
        ]
    )
