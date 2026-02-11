# Research Track 1 - Assignment 1

Student: Bahri Riadh
ID:8335614

## Project Description
This ROS 2 package implements a control system for Turtlesim with two nodes:

1.  **UI Node (Python):** A remote controller that allows the user to spawn a second turtle and send velocity commands to a specific turtle.
2.  **Distance Monitor (C++):** A safety node that monitors the distance between `turtle1` and `turtle2`. It automatically stops the robots if:
    * They get too close (Collision avoidance).
    * They get too close to the boundaries (Wall avoidance).

## How to Run
To run the full simulation, open please 2 separate terminals.

### 1. Launch Simulation & Safety System
This single command starts the Turtlesim window, spawns the second turtle, and enables the Distance Monitor automatically.
```bash
ros2 launch assignment1_rt assignment1.launch.py
```

#### 2. Run the UI Controller
In a second terminal, start the remote control UI to send commands.
```bash
ros2 run assignment1_rt ui_node.py
```

## Files Structure
* `scripts/ui_node.py`: Python script handling user input and velocity commands.
* `scripts/turtle_spawn.py`: Python script to call the /spawn service.
* `src/distance_monitor.cpp`: C++ source code implementing the distance calculation and safety logic.
* `CMakeLists.txt`: Configuration for building the mixed C++/Python package.
* `package.xml`: Defines dependencies (rclcpp, rclpy, turtlesim, geometry_msgs, std_msgs).
* `launch/assignment1.launch.py`: Additional python script that launch/start our nodes at the same time .