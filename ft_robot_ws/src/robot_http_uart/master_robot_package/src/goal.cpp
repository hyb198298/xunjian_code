#include "ros/ros.h"
#include "move_base_msgs/MoveBaseAction.h"
#include "actionlib/client/simple_action_client.h"



int main(int argc, char** argv)
{
    ros::init(argc, argv, "move_base_publisher");
    ros::NodeHandle n;
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>  ac("move_base", true);

    // 等待直到action服务器准备好
    ROS_INFO("Waiting for the move_base action server to start.");
    if (!ac.waitForServer(ros::Duration(5.0))) {
        ROS_ERROR("Server not available!");
        return -1;
    }
    ROS_INFO("Server started!");

    // 创建目标Pose消息
    move_base_msgs::MoveBaseGoal goal;
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();
    goal.target_pose.pose.position.x = 3.0;
    goal.target_pose.pose.position.y = 1.0;
    goal.target_pose.pose.orientation.w = 1.0;

    // 发布导航目标
    ROS_INFO("Sending goal to move_base action server.");
    ac.sendGoal(goal);

    // 等待直到达到目标或超时
    bool finished_before_timeout = ac.waitForResult(ros::Duration(22.0));

    if (finished_before_timeout) {
        if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
            ROS_INFO("Navigation reached the goal position!");
        } else {
            ROS_WARN("Navigation failed or timed out");
        }
    } else {
        ROS_WARN("Timed out waiting for the result!");
    }



    // 保持节点运行，直到用户中断
    ros::spin();

    return 0;
}