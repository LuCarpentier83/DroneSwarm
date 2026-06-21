from launch import LaunchDescription
from launch.actions import GroupAction
from launch_ros.actions import Node, PushRosNamespace

def generate_launch_description():
    robot1 = GroupAction([
        PushRosNamespace('robot1'),
        Node(package='simulation_pkg', executable='fake_odom_node'),
        Node(package='autonomous_drone', executable='point_nav_node'),
    ])

    robot2 = GroupAction([
        PushRosNamespace('robot2'),
        Node(package='simulation_pkg', executable='fake_odom_node'),
        Node(package='autonomous_drone', executable='point_nav_node'),
    ])

    return LaunchDescription([robot1, robot2])