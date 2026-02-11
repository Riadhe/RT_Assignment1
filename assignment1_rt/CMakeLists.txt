cmake_minimum_required(VERSION 3.8)
project(assignment1_rt)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()
#find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(turtlesim REQUIRED)
find_package(assignment1_custom_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
#build the  c++ Node
add_executable(distance_monitor src/distance_monitor.cpp)
ament_target_dependencies(distance_monitor rclcpp std_msgs turtlesim assignment1_custom_msgs geometry_msgs)
install(TARGETS
  distance_monitor

  DESTINATION lib/${PROJECT_NAME}
)
#install the python Node
install(PROGRAMS
  scripts/ui_node.py
  scripts/turtle_spawn.py
  DESTINATION lib/${PROJECT_NAME}
)
# Install Launch Files
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
