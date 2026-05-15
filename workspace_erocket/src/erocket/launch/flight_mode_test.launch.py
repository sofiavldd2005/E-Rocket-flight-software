"""
Launch the flight mode node alongside its test suite to verify state machine transitions.
"""

from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():

    micro_ros_agent = ExecuteProcess(
        cmd=[[
            'micro-ros-agent udp4 --port 8888 -v '
        ]],
        shell=True
    )

    # The main flight mode state machine node
    flight_mode_node = Node(
        package='erocket',
        executable='flight_mode',
        output='screen',
        shell=True,
        parameters=[
            PathJoinSubstitution([
                FindPackageShare('erocket'), 'config', 'offboard.yaml']),
        ],
    )

    # Node responsible for running the test suite against flight_mode
    flight_mode_test_node = Node(
        package='erocket',
        executable='flight_mode_test',
        output='screen',
        shell=True,
        parameters=[
            PathJoinSubstitution([
                FindPackageShare('erocket'), 'config', 'offboard.yaml']),
        ],
    )

    return LaunchDescription([
        # The micro-ROS agent is required to translate PX4 messages to ROS 2 topics
        micro_ros_agent,
        flight_mode_node,
        flight_mode_test_node,
    ])
