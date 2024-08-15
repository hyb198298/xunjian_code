#! /bin/bash
sleep 10;
echo '点云地图启动'; 
source /opt/ros/melodic/setup.bash; 
echo '加载运行环境';
source /home/ft/ft_robot_ws/devel/setup.bash; 
roslaunch laserscan_to_pointcloud laserscan_to_pointcloud.launch 
