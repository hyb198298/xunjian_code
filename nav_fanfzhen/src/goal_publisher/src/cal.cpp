#include "ros/ros.h"
#include "actionlib_msgs/GoalID.h"

int main(int argc, char **argv)
{
    // 初始化ROS节点
    ros::init(argc, argv, "cancel_move_base_node");

    // 创建节点句柄
    ros::NodeHandle nh;

    // 创建一个Publisher对象，用于发布GoalID消息
    ros::Publisher pub = nh.advertise<actionlib_msgs::GoalID>("/move_base/cancel", 10);

    // 等待节点连接到ROS master
    ros::spinOnce();
    ros::Duration(1).sleep();

    // 创建一个空的GoalID消息
    actionlib_msgs::GoalID msg;

    // 发布消息
    pub.publish(msg);

    // 等待一段时间，确保消息被发布
    ros::spinOnce();
    ros::Duration(1).sleep();

    // 正常退出
    return 0;
}