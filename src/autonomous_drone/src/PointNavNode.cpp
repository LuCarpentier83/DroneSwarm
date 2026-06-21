#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rclcpp.hpp"

using std::placeholders::_1;

class PointNavNode : public rclcpp::Node
{
  public:
    PointNavNode() : Node("point_nav_node")
    {
        cmd_pub_ = create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        odom_sub_ = create_subscription<nav_msgs::msg::Odometry>(
            "odom", 10, std::bind(&PointNavNode::odom_callback, this, _1));
    }

  private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;

    double target_x_ = 3.0;
    double target_y_ = 2.0;
    double distance_threshold = 0.3;

    void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
    {
        auto position = msg;
        double dx = target_x_ - position->pose.pose.position.x;
        double dy = target_y_ - position->pose.pose.position.y;
        double distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));

        geometry_msgs::msg::Twist cmd;

        if (distance <= distance_threshold) {
            RCLCPP_INFO(get_logger(), "Target reached");
            cmd_pub_->publish(cmd);
            return;
        }

        double target_yaw = std::atan2(dy, dx);
        double current_yaw =
            2.0 * std::atan2(msg->pose.pose.orientation.z, msg->pose.pose.orientation.w);

        double yaw_error = target_yaw - current_yaw;
        while (yaw_error > M_PI)
            yaw_error -= 2 * M_PI;
        while (yaw_error < -M_PI)
            yaw_error += 2 * M_PI;

        cmd.angular.z = 1.0 * yaw_error;
        cmd.linear.x = (std::abs(yaw_error) < 0.3) ? 0.4 : 0.0;

        cmd_pub_->publish(cmd);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PointNavNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}