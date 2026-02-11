#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from assignment1_custom_msgs.srv import GetAvgVel, ChangeThreshold

def main(args=None):
    rclpy.init(args=args)
    node = rclpy.create_node('ui_node')

    print("--- TurtleBot Control UI ---")
    print("Instructions: Enter 'turtle1' or 'turtle2'.")
    print("Press CTRL+C to exit.")

    while rclpy.ok():
        try:
            
            t_input = input("\nEnter Turtle Name (turtle1/turtle2): ").lower()
            
            if t_input not in ['turtle1', 'turtle2']:
                print("Invalid Name. Please enter 'turtle1' or 'turtle2'.")
                continue

            linear_vel = float(input("Enter Linear Velocity: "))
            angular_vel = float(input("Enter Angular Velocity: "))

            # Create publisher for that specific turtle
            topic_name = f'/{t_input}/cmd_vel'
            publisher = node.create_publisher(Twist, topic_name, 10)
                    # Create clients to communicate with the services provided by the robot_controller
            self.cli_thr = self.create_client(ChangeThreshold, 'change_threshold')
            
            # Send "Move" command
            twist_msg = Twist()
            twist_msg.linear.x = linear_vel
            twist_msg.angular.z = angular_vel
            publisher.publish(twist_msg)
            print(f"Moving {t_input} for 1 second...")

            # Wait
            time.sleep(1.0)

            # Send "Stop" command
            twist_msg.linear.x = 0.0
            twist_msg.angular.z = 0.0
            publisher.publish(twist_msg)
            print(f"{t_input} stopped.")

        except ValueError:
            print("Error: Please enter numeric values for velocity.")
        except KeyboardInterrupt:
            print("\nExiting UI...")
            break

    node.destroy_node()
    rclpy.shutdown()
def set_threshold(self):
        try:
            # Get user input for new distance
            val = float(input("Enter new Threshold : "))
            
            if not self.cli_thr.wait_for_service(1.0):
                print("Service not available!")
                return
            
            # Prepare the request
            req = ChangeThreshold.Request()
            req.new_threshold = val
            
            # Send and wait for response
            future = self.cli_thr.call_async(req)
            rclpy.spin_until_future_complete(self, future)
            print(f"--> Threshold updated to {val} successfully!")
            
        except ValueError:
            print("Invalid Number!")
if __name__ == '__main__':
    main()