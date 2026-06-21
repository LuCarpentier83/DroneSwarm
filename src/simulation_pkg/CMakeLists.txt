cmake_minimum_required(VERSION 3.20)
project(simulation_pkg)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nav_msgs REQUIRED)

add_executable(fake_odom_node src/FakeOdomNode.cpp)
target_link_libraries(fake_odom_node
        rclcpp::rclcpp std_msgs::std_msgs nav_msgs::nav_msgs
)

install(TARGETS fake_odom_node DESTINATION lib/${PROJECT_NAME})

ament_package()
