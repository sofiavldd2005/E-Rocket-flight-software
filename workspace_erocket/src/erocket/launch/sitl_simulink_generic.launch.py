"""
Launch the generic custom controller in a Software-In-The-Loop (SITL) simulation environment.
"""

from launch import LaunchDescription
from launch.actions import ExecuteProcess
from launch_ros.actions import Node
import os

# Get the parent directory of the directory containing this launch file
launch_file_dir = os.path.dirname(os.path.realpath(__file__))
config_file = os.path.realpath(os.path.join(
    launch_file_dir,
    '../../../../../src/erocket/config/offboard.yaml'
))

# SITL simulation parameters!
# Locks out servos and motors to prevent accidental hardware actuation.
additional_params = {
    'servo_active': False,
    'motor_active': False,
}

def generate_launch_description():

    # Start the custom generic controller
    controller_generic_node = Node(
        package='erocket',
        executable='controller_generic',
        output='screen',
        shell=True,
        parameters=[config_file, additional_params],
    )

    mission_node = Node(
        package='erocket',
        executable='mission',
        output='screen',
        shell=True,
        parameters=[config_file, additional_params],
    )

    # Start the mock flight mode to safely simulate hardware state changes
    mock_flight_mode_node = Node(
        package='erocket',
        executable='mock_flight_mode',
        output='screen',
        shell=True,
        arguments=['--ros-args', '--log-level', 'warn'],
        parameters=[config_file, additional_params],
    )


    return LaunchDescription([
        controller_generic_node,
        mission_node,
        mock_flight_mode_node,
    ])
