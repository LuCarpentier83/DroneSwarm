# ROS2 Lyrical + CLion Setup Guide

Complete configuration guide for developing ROS2 C++ packages with CLion on Ubuntu 26.04 (Resolute) / ROS2 Lyrical Luth. This document captures the full setup procedure and the issues encountered along the way, so it can be reproduced quickly for future packages.

## Table of contents

- [Prerequisites](#prerequisites)
- [1. Install ROS2 Lyrical](#1-install-ros2-lyrical)
- [2. Create the workspace](#2-create-the-workspace)
- [3. Create a package](#3-create-a-package)
- [4. Configure CLion to find ROS2](#4-configure-clion-to-find-ros2)
- [5. Lyrical/Rolling quirk: `ament_target_dependencies` removed](#5-lyricalrolling-quirk-ament_target_dependencies-removed)
- [6. Reference CMakeLists.txt](#6-reference-cmakeliststxt)
- [CLion build vs colcon build](#clion-build-vs-colcon-build)
- [Reusing this setup for a new package](#reusing-this-setup-for-a-new-package)
- [Error log](#error-log)

## Prerequisites

- Ubuntu 26.04 LTS (Resolute)
- ROS2 Lyrical Luth (LTS, supported until 2031)
- CLion installed via JetBrains Toolbox

## 1. Install ROS2 Lyrical

```bash
sudo apt update && sudo apt install curl -y
export ROS_APT_SOURCE_VERSION=$(curl -s https://api.github.com/repos/ros-infrastructure/ros-apt-source/releases/latest | grep -F "tag_name" | awk -F'"' '{print $4}')
curl -L -o /tmp/ros2-apt-source.deb "https://github.com/ros-infrastructure/ros-apt-source/releases/download/${ROS_APT_SOURCE_VERSION}/ros2-apt-source_${ROS_APT_SOURCE_VERSION}.$(. /etc/os-release && echo ${UBUNTU_CODENAME:-${VERSION_CODENAME}})_all.deb"
sudo dpkg -i /tmp/ros2-apt-source.deb

sudo apt update
sudo apt install ros-lyrical-desktop ros-dev-tools python3-colcon-common-extensions
```

## 2. Create the workspace

```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
```

## 3. Create a package

```bash
ros2 pkg create --build-type ament_cmake my_package --dependencies rclcpp geometry_msgs nav_msgs
```

Adjust the `--dependencies` list based on what the package actually needs (at minimum `rclcpp` for a C++ node).

## 4. Configure CLion to find ROS2

### The problem

CLion runs its own CMake internally. If you launch it normally (from the application menu), it has no idea where `rclcpp`, `ament_cmake`, etc. live — configuration fails.

### The reliable fix: environment variables in the CMake profile

Instead of depending on how CLion happens to be launched, set the variables directly in the project's CMake profile settings — this works no matter how CLion is started.

**Step 1 — Get the values**, in a terminal where ROS2 has been sourced:

```bash
source /opt/ros/lyrical/setup.bash
# if the workspace has already been built at least once:
source ~/ros2_ws/install/setup.bash

echo "AMENT_PREFIX_PATH=$AMENT_PREFIX_PATH"
echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
echo "PATH=$PATH"
echo "PYTHONPATH=$PYTHONPATH"
```

**Step 2 — Paste them into CLion**:
`Settings → Build, Execution, Deployment → CMake` → select the profile (Debug) → **Environment** field → paste the 4 lines (one per line, `KEY=value` format).

**Step 3 — Apply**:
`Apply` → right-click `CMakeLists.txt` in the project tree → **Reset Cache and Reload Project**.

### Why these 4 variables specifically

| Variable | What it's for |
|---|---|
| `PYTHONPATH` | Lets `ament_cmake`'s internal Python script (template generation) find the `ament_package` module |
| `AMENT_PREFIX_PATH` | Used by the ament resource index to locate typesupport implementations (`rosidl_typesupport_c`, RMW...) — distinct from `CMAKE_PREFIX_PATH` |
| `LD_LIBRARY_PATH` | Needed at runtime to load ROS2 `.so` files (otherwise a `cannot open shared object file` error on launch) |
| `PATH` | So ROS2/colcon CLI tools are found if needed during the build |

### If Run/Debug fails after a successful build

The CMake profile and the Run/Debug configuration are two separate things in CLion. If the executable builds fine but crashes on launch with a missing library error: `Edit Configurations` → **Environment variables** field → add at least `LD_LIBRARY_PATH` there too.

## 5. Lyrical/Rolling quirk: `ament_target_dependencies` removed

Starting with Lyrical Luth (and Rolling), the `ament_target_dependencies()` macro — used in almost every existing ROS2 tutorial — has been **completely removed**, not just deprecated. Typical error:

```
CMake Error: Unknown CMake command "ament_target_dependencies".
```

**Modern replacement** using `target_link_libraries()` with the named targets each package exports:

```cmake
# Old (no longer works on Lyrical/Rolling)
ament_target_dependencies(my_node rclcpp geometry_msgs nav_msgs)

# New
target_link_libraries(my_node
  rclcpp::rclcpp
  ${geometry_msgs_TARGETS}
  ${nav_msgs_TARGETS}
)
```

General rule: libraries with a classic namespaced target (`rclcpp::rclcpp`) are linked directly by name; message packages use the `${<package_name>_TARGETS}` variable.

## 6. Reference CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.8)
project(my_package)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)

add_executable(my_node src/my_node.cpp)
target_link_libraries(my_node
  rclcpp::rclcpp
  ${geometry_msgs_TARGETS}
  ${nav_msgs_TARGETS}
)

install(TARGETS
  my_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
```

## CLion build vs colcon build

- **CLion build** (hammer icon / `Ctrl+F9`): compiles into its own folder (`cmake-build-debug/`), lets you run/debug directly in the IDE with breakpoints. Covers 95% of day-to-day work.
- **`colcon build`** (terminal, from the workspace root): installs the package into `install/`, required for `ros2 run`, `ros2 launch`, or if another package depends on yours.

Keep `colcon build` for full integration testing (Gazebo, multi-node, multi-package), and CLion for fast iteration on the code itself.

## Reusing this setup for a new package

`PYTHONPATH`, `LD_LIBRARY_PATH`, and `PATH` are identical regardless of the package — copy/paste them straight into the new project's CMake profile.

`AMENT_PREFIX_PATH` may need updating if the new package depends on another custom package in the workspace:
1. `colcon build` the dependency package
2. Re-source `~/ros2_ws/install/setup.bash`
3. Run `echo $AMENT_PREFIX_PATH` again to get the updated value
4. Update the variable in the new package's CMake profile

## Error log

| Error | Cause | Fix |
|---|---|---|
| `ModuleNotFoundError: No module named 'ament_package'` | `PYTHONPATH` missing from the environment CLion uses for CMake configuration | Add `PYTHONPATH` to the CMake profile (Environment) |
| `CMake Error: No 'rosidl_typesupport_c' found` | `AMENT_PREFIX_PATH` missing — used for the ament resource index (typesupports), distinct from `CMAKE_PREFIX_PATH` | Add `AMENT_PREFIX_PATH` to the CMake profile |
| `CMake Error: Unknown CMake command "ament_target_dependencies"` | Macro removed in Lyrical/Rolling, not just deprecated | Replace with `target_link_libraries()` using named targets (`rclcpp::rclcpp`, `${pkg_TARGETS}`) |
| `error while loading shared libraries: ... cannot open shared object file` on Run/Debug | `LD_LIBRARY_PATH` not picked up by the Run/Debug configuration (separate from the CMake profile) | Add `LD_LIBRARY_PATH` under Edit Configurations → Environment variables |