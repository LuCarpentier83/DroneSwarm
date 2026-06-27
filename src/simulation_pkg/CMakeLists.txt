cmake_minimum_required(VERSION 3.20)
project(simulation_pkg)

if (NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif ()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(nav_msgs REQUIRED)

add_executable(fake_odom_node src/FakeOdomNode.cpp)

add_executable(test_grid src/test_grid.cpp src/Grid.cpp)
target_include_directories(test_grid PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(fake_odom_node
        rclcpp::rclcpp std_msgs::std_msgs nav_msgs::nav_msgs
)

install(TARGETS fake_odom_node DESTINATION lib/${PROJECT_NAME})
install(TARGETS test_grid DESTINATION lib/${PROJECT_NAME})

ament_package()
