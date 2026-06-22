cmake_minimum_required(VERSION 3.20)
project(environment_interfaces)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
        "msg/Target.msg"
        "msg/Obstacle.msg"
)

ament_package()