#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/float32.hpp"
#include <cmath>

using std::placeholders::_1;

class DistanceMonitor : public rclcpp::Node
{
public:
  DistanceMonitor() : Node("distance_monitor")
  {
    // 1. Subscribers
    turtle1_subscriber_ = this->create_subscription<turtlesim::msg::Pose>(
        "/turtle1/pose", 10, std::bind(&DistanceMonitor::callback_turtle1, this, _1));
    turtle2_subscriber_ = this->create_subscription<turtlesim::msg::Pose>(
        "/turtle2/pose", 10, std::bind(&DistanceMonitor::callback_turtle2, this, _1));

    // 2. Publishers
    turtle1_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    turtle2_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle2/cmd_vel", 10);
    distance_publisher_ = this->create_publisher<std_msgs::msg::Float32>("/distance_traveled", 10);

    // 3. Timer
    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(100), std::bind(&DistanceMonitor::check_safety, this));

    // Initialize
    turtle1_pose_.x = -1.0;
    turtle2_pose_.x = -1.0;
  }

private:
    turtlesim::msg::Pose turtle1_pose_, turtle2_pose_;
    bool turtle1_received_ = false;
    bool turtle2_received_ = false;

    // Callbacks
    void callback_turtle1(const turtlesim::msg::Pose::SharedPtr msg)
    {
        turtle1_pose_ = *msg;
        turtle1_received_ = true;
    }

    void callback_turtle2(const turtlesim::msg::Pose::SharedPtr msg)
    {
        turtle2_pose_ = *msg;
        turtle2_received_ = true;   
    }

    void stopTurtle(rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pub)
    {
        auto stop_msg = geometry_msgs::msg::Twist();
        stop_msg.linear.x = 0.0;
        stop_msg.angular.z = 0.0;
        pub->publish(stop_msg);
    }

    void check_safety()
    {
        if (!turtle1_received_ || !turtle2_received_) return;

        // A. Calculate Distance
        float dist = std::sqrt(std::pow(turtle1_pose_.x - turtle2_pose_.x, 2) + 
                               std::pow(turtle1_pose_.y - turtle2_pose_.y, 2));

        // Publish Distance
        auto dist_msg = std_msgs::msg::Float32();
        dist_msg.data = dist;
        distance_publisher_->publish(dist_msg);

        // B. Check Collision
        if (dist < 1.0) {
            RCLCPP_WARN(this->get_logger(), "COLLISION RISK! Distance: %f", dist);
            stopTurtle(turtle1_publisher_);
            stopTurtle(turtle2_publisher_);
        }

        // C. Check Boundaries
        // Turtle 1
        if (turtle1_pose_.x > 10.0 || turtle1_pose_.x < 1.0 || 
            turtle1_pose_.y > 10.0 || turtle1_pose_.y < 1.0) {
            RCLCPP_WARN(this->get_logger(), "Turtle 1 hit boundary!");
            stopTurtle(turtle1_publisher_);
        }
        // Turtle 2
        if (turtle2_pose_.x > 10.0 || turtle2_pose_.x < 1.0 || 
            turtle2_pose_.y > 10.0 || turtle2_pose_.y < 1.0) {
            RCLCPP_WARN(this->get_logger(), "Turtle 2 hit boundary!");
            stopTurtle(turtle2_publisher_);
        }
    }

    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr turtle1_subscriber_;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr turtle2_subscriber_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr turtle1_publisher_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr turtle2_publisher_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr distance_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DistanceMonitor>());
  rclcpp::shutdown();
  return 0;
}