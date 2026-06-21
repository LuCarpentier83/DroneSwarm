cmake_minimum_required(VERSION 3.20)
project(autonomous_drone)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(nav_msgs REQUIRED)

add_executable(point_nav_node src/PointNavNode.cpp)
target_link_libraries(point_nav_node
        rclcpp::rclcpp std_msgs::std_msgs sensor_msgs::sensor_msgs tf2::tf2 nav_msgs::nav_msgs
)

install(TARGETS point_nav_node DESTINATION lib/${PROJECT_NAME})

ament_package()
