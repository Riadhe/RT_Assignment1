from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # 1. Start Turtlesim
        Node(
            package='turtlesim',
            executable='turtlesim_node',
            name='sim'
        ),
        # 2. Spawn the second turtle
        Node(
            package='assignment1_rt',
            executable='turtle_spawn.py',
            name='spawner'
        ),
        # 3. Start the Distance Monitor
        Node(
            package='assignment1_rt',
            executable='distance_monitor',
            name='monitor',
            output='screen'  # This ensures you see the COLLISION warnings
        ),
    ])