cmake_minimum_required(VERSION 3.20)
project(autonomous_drone)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(autonomous_drone src/main.cpp)
target_link_libraries(autonomous_drone
        rclcpp::rclcpp
)

install(TARGETS autonomous_drone DESTINATION lib/${PROJECT_NAME})

ament_package()
