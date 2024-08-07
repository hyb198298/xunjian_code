#include "laserscan_to_pointcloud/laserscan_to_pointcloud.h"
#include <limits>

#include <pcl/filters/passthrough.h>

ScanToPointCloud::ScanToPointCloud() 
{
    std::cout<<"ScanToPointCloud::ScanToPointCloud()"<<std::endl;

 //雷达安装和小车没有对其,选择了180这里没有用坐标旋转,需要进一步改进
    nh.getParam("/laserscan_to_pointcloud/front_x_down", front_x_down);
    nh.getParam("/laserscan_to_pointcloud/front_x_up", front_x_up);
    nh.getParam("/laserscan_to_pointcloud/front_y_down", front_y_down);
    nh.getParam("/laserscan_to_pointcloud/front_y_up", front_y_up);

    nh.getParam("/laserscan_to_pointcloud/back_x_down",back_x_down);
    nh.getParam("/laserscan_to_pointcloud/back_x_up",back_x_up);
    nh.getParam("/laserscan_to_pointcloud/back_y_down",back_y_down);
    nh.getParam("/laserscan_to_pointcloud/back_y_up", back_y_up);

    // front_x_down =  -front_x_down;
    // front_x_up =  -front_x_up;
    // front_y_down = -front_y_down;
    // front_y_up = -front_y_up;

    // back_x_down = -back_x_down;
    // back_x_up = -back_x_up;
    // back_y_down =   -back_y_down;
    // back_y_up =  -back_y_up;



    std::cout<<"front : "<<front_x_down<<","<<front_x_up<<","<<front_y_down<<","<<front_y_up<<std::endl;
    std::cout<<"back : "<<back_x_down<<","<<back_x_up<<","<<back_y_down<<","<<back_y_up<<std::endl;


    laser_scan_subscriber_ = node_handle_.subscribe("scan", 1, &ScanToPointCloud::ScanCallback, this);

    pointcloud2_publisher_ = node_handle_.advertise<pcl::PointCloud<pcl::PointXYZ>>("point_cloud", 1, this);
    pointcloud2_publisher_1 = node_handle_.advertise<pcl::PointCloud<pcl::PointXYZ>>("point_cloud1", 1, this);
    // pointcloud2_publisher_2 = node_handle_.advertise<pcl::PointCloud<pcl::PointXYZ>>("point_cloud2", 1, this);

    invalid_point_.x = std::numeric_limits<float>::quiet_NaN();
    invalid_point_.y = std::numeric_limits<float>::quiet_NaN();
    invalid_point_.z = std::numeric_limits<float>::quiet_NaN();
}

ScanToPointCloud::~ScanToPointCloud()
{
}

void ScanToPointCloud::ScanCallback(const sensor_msgs::LaserScan::ConstPtr &scan_msg)
{
     //std::cout<<"ScanToPointCloud::ScanCallback()"<<std::endl;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_msg = boost::shared_ptr<pcl::PointCloud<pcl::PointXYZ>>(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_msg1 = boost::shared_ptr<pcl::PointCloud<pcl::PointXYZ>>(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_msg2 = boost::shared_ptr<pcl::PointCloud<pcl::PointXYZ>>(new pcl::PointCloud<pcl::PointXYZ>());

    cloud_msg->points.resize(scan_msg->ranges.size());

    for (unsigned int i = 0; i < scan_msg->ranges.size(); ++i)
    {
        pcl::PointXYZ &point_tmp = cloud_msg->points[i];
        
        float range = scan_msg->ranges[i];

        if (!std::isfinite(range))
        {
            point_tmp = invalid_point_;
            continue;
        }

        if (range > scan_msg->range_min && range < scan_msg->range_max)
        {
            float angle = scan_msg->angle_min + i * scan_msg->angle_increment;
            point_tmp.x = range * cos(angle);
            point_tmp.y = range * sin(angle);
            point_tmp.z = 0.0;
        }
        else
            point_tmp = invalid_point_;
    }

    cloud_msg->width = scan_msg->ranges.size();
    cloud_msg->height = 1;
    cloud_msg->is_dense = false;

    cloud_msg1 =(* cloud_msg).makeShared();
     cloud_msg2 =(* cloud_msg).makeShared();
     std::cout<<" cloud_msg->points.size() = "<< cloud_msg->points.size()<<","<<cloud_msg1->points.size()<<","<<cloud_msg2->points.size()<<std::endl;
//----------------------------------
//车前滤波
    pcl::PassThrough<pcl::PointXYZ> pass;
    pass.setInputCloud(cloud_msg1);      // 1. 设置输入源
    pass.setFilterFieldName("y");       // 2. 设置过滤域名
    pass.setFilterLimits(front_y_down, front_y_up);     // 3. 设置过滤范围
	//pass.setFilterLimitsNegative(true); // 设置获取Limits之外的内容
    pass.filter(*cloud_msg1);       // 4. 执行过滤，并将结果输出到cloud_filtered
    pass.setFilterFieldName("x");       // 2. 设置过滤域名
    pass.setFilterLimits(-front_x_up, -front_x_down);     // 3. 设置过滤范围
     pass.filter(*cloud_msg1);       // 4. 执行过滤，并将结果输出到cloud_filtered
    pcl_conversions::toPCL(scan_msg->header, cloud_msg1->header);
    pointcloud2_publisher_.publish(cloud_msg1);

    //----------------------------------
//车后滤波
    // pcl::PassThrough<pcl::PointXYZ> pass;
    pass.setInputCloud(cloud_msg2);      // 1. 设置输入源
    pass.setFilterFieldName("y");       // 2. 设置过滤域名
    pass.setFilterLimits(back_y_down, back_y_up);     // 3. 设置过滤范围
	//pass.setFilterLimitsNegative(true); // 设置获取Limits之外的内容
    pass.filter(*cloud_msg2);       // 4. 执行过滤，并将结果输出到cloud_filtered
    pass.setFilterFieldName("x");       // 2. 设置过滤域名
    pass.setFilterLimits(-back_x_up, -back_x_down);     // 3. 设置过滤范围
     pass.filter(*cloud_msg2);       // 4. 执行过滤，并将结果输出到cloud_filtered
    


    pcl_conversions::toPCL(scan_msg->header, cloud_msg2->header);
    pointcloud2_publisher_1.publish(cloud_msg2);

    std::cout<<" cloud_msg->points.size() = "<< cloud_msg->points.size()<<","<<cloud_msg1->points.size()<<","<<cloud_msg2->points.size()<<std::endl;

    return ;
//-----------------------------------


// //车前滤波
//     pcl::PassThrough<pcl::PointXYZ> pass1;
//     pass1.setInputCloud(cloud_msg1);      // 1. 设置输入源
//     pass1.setFilterFieldName("y");       // 2. 设置过滤域名
//     pass1.setFilterLimits(front_y_down, front_y_up);     // 3. 设置过滤范围
// 	//pass.setFilterLimitsNegative(true); // 设置获取Limits之外的内容
//     pass1.filter(*cloud_msg1);       // 4. 执行过滤，并将结果输出到cloud_filtered
//     pass1.setFilterFieldName("x");       // 2. 设置过滤域名
//     pass1.setFilterLimits(front_x_down, front_x_up);     // 3. 设置过滤范围
//      pass1.filter(*cloud_msg1);       // 4. 执行过滤，并将结果输出到cloud_filtered

// //车后滤波
//     pcl::PassThrough<pcl::PointXYZ> pass2;
//     pass2.setInputCloud(cloud_msg2);      // 1. 设置输入源
//     pass2.setFilterFieldName("y");       // 2. 设置过滤域名
//     pass2.setFilterLimits(back_y_down, back_y_up);     // 3. 设置过滤范围
// 	//pass.setFilterLimitsNegative(true); // 设置获取Limits之外的内容
//     pass2.filter(*cloud_msg2);       // 4. 执行过滤，并将结果输出到cloud_filtered
//     pass2.setFilterFieldName("x");       // 2. 设置过滤域名
//     pass2.setFilterLimits(back_x_down, back_x_up);     // 3. 设置过滤范围
//      pass2.filter(*cloud_msg2);       // 4. 执行过滤，并将结果输出到cloud_filtered

//      std::cout<<" cloud_msg1->points.size() = "<< cloud_msg1->points.size()<<std::endl;
// //-------------------------------------------

//     pcl_conversions::toPCL(scan_msg->header, cloud_msg1->header);
//     pcl_conversions::toPCL(scan_msg->header, cloud_msg2->header);
//     pointcloud2_publisher_1.publish(cloud_msg1);
//     pointcloud2_publisher_2.publish(cloud_msg2);

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "laserscan_to_pointcloud");

    ScanToPointCloud laserscan_to_pointcloud;

    ros::spin();

    return 0;
}
