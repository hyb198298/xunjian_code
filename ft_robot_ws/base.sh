#! /bin/bash
sleep 2;
echo '底盘启动'; 
source /opt/ros/melodic/setup.bash; 
echo '加载运行环境';
source /home/ft/ft_robot_ws/devel/setup.bash; 

roslaunch turn_on_wheeltec_robot base_serial.launch;





