#! /bin/bash
sleep 1;
echo '-------';
source /opt/ros/melodic/setup.bash;
echo '--------';
source /home/ft/ft_robot_ws/devel/setup.bash;

roslaunch laserscan_to_pointcloud laserscan_to_pointcloud.launch;
