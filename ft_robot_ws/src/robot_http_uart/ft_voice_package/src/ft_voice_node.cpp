#include "ros/ros.h"
#include "std_msgs/String.h"
#include<iostream>
#include<cstdlib>

void chatterCallback(const std_msgs::String::ConstPtr& msg) {
   std::string voice = "/home/ft/ft_robot_ws/voice/" + msg->data;
   // 使用更安全的命令执行方式
   if (system(("mplayer " + voice).c_str()) != 0) {
     ROS_ERROR("Failed to play file: %s", voice.c_str());
   }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;

    // 创建一个订阅者对象，订阅名为"chatter"的话题
    ros::Subscriber sub = n.subscribe("chatter", 10, chatterCallback);

    ros::spin(); // 等待回调函数被调用

    return 0;
}