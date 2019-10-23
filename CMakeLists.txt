cmake_minimum_required(VERSION 2.8.3)
project(image_to_map)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

set(OpenCV_DIR /opt/ros/kinetic/share/OpenCV-3.3.1-dev)
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  OpenCV REQUIRED
  image_transport
  geometry_msgs
  nav_msgs
  roscpp
  std_msgs
  sensor_msgs
  tf2
)


## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   geometry_msgs#   nav_msgs#   std_msgs
# )

catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES image_to_map
  CATKIN_DEPENDS geometry_msgs nav_msgs roscpp std_msgs sensor_msgs
  DEPENDS system_lib
)

include_directories(
 ./include
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} src/main.cpp src/image_to_map_node.cpp )

add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
)
