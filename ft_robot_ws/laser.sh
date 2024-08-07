#! /bin/bash
echo '雷达启动'; 
source /opt/ros/melodic/setup.bash; 
echo '加载运行环境';
source /home/ft/ft_robot_ws/devel/setup.bash; 

roslaunch lslidar_driver lslidar_serial.launch;





