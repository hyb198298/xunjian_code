#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <pcl_ros/point_cloud.h>    
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include<iostream>
#include "std_msgs/Int32.h"
class ScanToPointCloud
{

private:
    ros::NodeHandle node_handle_;
    ros::Subscriber laser_scan_subscriber_;
    ros::Publisher pointcloud2_publisher_;
    ros::Publisher pointcloud2_publisher_1;
    ros::Publisher pointcloud2_publisher_2;
    pcl::PointXYZ invalid_point_;
    ros::Publisher publisher ;


    ros::NodeHandle nh;

    float front_x_down;
    float front_x_up;
    float front_y_down;
    float front_y_up;

    float back_x_down;
    float back_x_up;
    float back_y_down;
    float back_y_up;

public:
    ScanToPointCloud();

    ~ScanToPointCloud();

    void ScanCallback(const sensor_msgs::LaserScan::ConstPtr& scan_msg);
};

