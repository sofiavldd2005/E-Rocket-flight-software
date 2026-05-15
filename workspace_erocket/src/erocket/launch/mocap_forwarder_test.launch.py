"""
Launch the MoCap forwarder node and its associated test node to verify translation.
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

    # Node that bridges VRPN tracking data to PX4 VehicleOdometry
    mocap_forwarder_node = Node(
        package='erocket',
        executable='mocap_forwarder',
        output='screen',
        shell=True,
        parameters=[
            PathJoinSubstitution([
                FindPackageShare('erocket'), 'config', 'offboard.yaml']),
        ],
    )

    # Node to test the forwarder logic
    mocap_forwarder_test_node = Node(
        package='erocket',
        executable='mocap_forwarder_test',
        output='screen',
        shell=True,
        parameters=[
            PathJoinSubstitution([
                FindPackageShare('erocket'), 'config', 'offboard.yaml']),
        ],
    )

    return LaunchDescription([
        #micro_ros_agent,
        mocap_forwarder_node,
        mocap_forwarder_test_node,
    ])
