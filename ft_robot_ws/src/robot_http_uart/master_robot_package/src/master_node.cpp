#include "master.h"



int  main(int argc,char *argv[])
{
    ros::init(argc,argv,"my_master_node");
    ros::NodeHandle  nh;
    ROS_INFO("master_node  has  open!");
    Master  master;
    ros::spin();
    ros::shutdown();
    return 0;

}