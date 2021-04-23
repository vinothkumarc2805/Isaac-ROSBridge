#include <ros/ros.h>
// Include the appropriate message headers for your test case
#include <geometry_msgs/Pose2D.h>

// Sending random points for navigation testing
#include <random>

// A simple call back to acknowledge receiving a message
void navigationCallback(const geometry_msgs::Pose2D::ConstPtr& msg) {
    ROS_INFO("Heard: Isaac is at X:%f, Y:%f, Theta: %f", msg->x, msg->y, msg->theta);
}	

constexpr double pi = 3.14159265358979323846;

int main(int argc, char **argv) {
    // Select a random point to send as a goal pose
    std::random_device rd;
    std::mt19937_64 eng(rd());
    std::uniform_real_distribution<> dist_x(10, 30);
    std::uniform_real_distribution<> dist_y(10, 30);
    std::uniform_real_distribution<> dist_theta(-pi, pi);

    ros::init(argc, argv, "rosBridgeTest");

    ros::NodeHandle rosBridgeTestNode;

    // Channel to publish navigation requests to.
    // Replace with desired message and channel name for your test case
    ros::Publisher nav_pub = rosBridgeTestNode.advertise<geometry_msgs::Pose2D>("isaac_navigation2D_request", 1000);

    // Listen for pose/navigation updates.
    // Replace with desired message and channel name for your test case
    ros::Subscriber nav_sub = rosBridgeTestNode.subscribe("isaac_navigation2D_status", 1000, &navigationCallback);

    // Send a message and check updates every ten seconds
    ros::Rate loop_rate(1);

    while (ros::ok()) {
        // Generate a random goal location
        geometry_msgs::Pose2D pose_msg;
        pose_msg.x = dist_x(eng);
        pose_msg.y = dist_y(eng);
        pose_msg.theta = dist_theta(eng);

        ROS_INFO("Sent: Move Isaac to  X:%f, Y:%f, Theta: %f", pose_msg.x, pose_msg.y, pose_msg.theta);
        nav_pub.publish(pose_msg);

        // Manually spin to check messages
        ros::spinOnce();

        loop_rate.sleep();
    }
    return 0;
}
