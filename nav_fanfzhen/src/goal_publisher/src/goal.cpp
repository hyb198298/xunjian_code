// #include "ros/ros.h"
// #include "move_base_msgs/MoveBaseAction.h"
// #include "actionlib/client/simple_action_client.h"

// typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

// int main(int argc, char** argv)
// {
//     ros::init(argc, argv, "move_base_publisher");
//     ros::NodeHandle n;
//     MoveBaseClient ac("move_base", true);

//     // 等待直到action服务器准备好
//     ROS_INFO("Waiting for the move_base action server to start.");
//     ac.waitForServer(ros::Duration(5.0));

//     // 创建目标Pose消息
//     move_base_msgs::MoveBaseGoal goal;
//     goal.target_pose.header.frame_id = "map";
//     goal.target_pose.header.stamp = ros::Time::now();
//     goal.target_pose.pose.position.x =2.0;
//     goal.target_pose.pose.position.y =2.0;
//     goal.target_pose.pose.orientation.w = 1.0;

//     // 发布导航目标
//     ac.sendGoal(goal);

//     // 等待直到达到目标或超时
//     bool finished_before_timeout = ac.waitForResult(ros::Duration(60.0));

//     if (finished_before_timeout && ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
//     {
//         ROS_INFO("Navigation reached the goal position!");
//     }
//     else
//     {
//         ROS_INFO("Navigation failed or timed out");
//     }

//     // 取消导航任务
//     ac.cancelGoal();

//     // 保持节点运行，直到用户中断
//     ros::spin();

//     return 0;
// }
#include <ros/ros.h>
#include <sensor_msgs/PointCloud.h>
#include <cstdlib> // 包含C标准库函数，如rand()

int main(int argc, char **argv) {
    ros::init(argc, argv, "publish_point_cloud");
    ros::NodeHandle nh;

    // 创建一个发布者，用于发布PointCloud消息
    ros::Publisher pointcloud_pub = nh.advertise<sensor_msgs::PointCloud>("point_cloud", 1);

    // 创建PointCloud消息
    sensor_msgs::PointCloud point_cloud;
    point_cloud.header.stamp = ros::Time::now();
    point_cloud.header.frame_id = "odom";

    // 定义PointCloud中的点的数量
    size_t num_points = 10;

    // 为PointCloud消息添加PointField
    point_cloud.points.resize(num_points);
    point_cloud.channels.resize(0); // 没有使用额外的通道

    // 填充点数据
    for (size_t i = 0; i < num_points; ++i) {
        // 随机生成点的坐标
        point_cloud.points[i].x = 0.1 * (rand() / double(RAND_MAX)) - 0.05;
        point_cloud.points[i].y = 0.1 * (rand() / double(RAND_MAX)) - 0.05;
        point_cloud.points[i].z = 0.0; // 假设在xy平面上
    }

    // 设置循环频率
    ros::Rate loop_rate(1); // 每秒发布一次

    while (ros::ok()) {
        // 发布PointCloud消息
        pointcloud_pub.publish(point_cloud);

        // 等待下一个循环周期
        loop_rate.sleep();
    }

    // 程序结束
    return 0;
}