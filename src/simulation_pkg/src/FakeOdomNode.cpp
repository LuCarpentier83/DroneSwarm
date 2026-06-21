#include "rclcpp/rclcpp.hpp"
#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>

using std::placeholders::_1;
#define TIMER_MS 50

class FakeOdomNode : public rclcpp::Node

{
  public:
    FakeOdomNode() : Node("fake_odom_node")
    {
        odom_pub_ = create_publisher<nav_msgs::msg::Odometry>("odom", 10);
        cmd_sub_ = create_subscription<geometry_msgs::msg::Twist>(
            "cmd_vel", 10, std::bind(&FakeOdomNode::CmdCallback, this, _1));
        timer_ = create_timer(std::chrono::milliseconds(TIMER_MS),
                              std::bind(&FakeOdomNode::TimerCallback, this));
    }

  private:
    void CmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
    {
        auto cmd = msg;
        last_speed_ = cmd->linear.x;
        last_omega_ = cmd->angular.z;
    }
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_sub_;

    double x_ = 0.0;
    double y_ = 0.0;
    double theta_ = 0.0;
    double dt_ = TIMER_MS * 0.001;

    double last_speed_ = 0.0;
    double last_omega_ = 0.0;

    rclcpp::TimerBase::SharedPtr timer_;

    void TimerCallback()
    {
        x_ += last_speed_ * dt_ * cos(theta_);
        y_ += last_speed_ * dt_ * sin(theta_);
        theta_ += last_omega_ * dt_;

        nav_msgs::msg::Odometry odom;
        odom.header.stamp = get_clock()->now();
        odom.header.frame_id = "odom";
        odom.child_frame_id = "base_link";

        odom.pose.pose.position.x = x_;
        odom.pose.pose.position.y = y_;

        odom.pose.pose.orientation.z = std::sin(theta_ / 2);
        odom.pose.pose.orientation.w = std::cos(theta_ / 2);

        odom_pub_->publish(odom);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<FakeOdomNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}