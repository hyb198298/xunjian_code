#! /bin/bash
sleep 5;
echo 'move_base启动'; 
source /opt/ros/melodic/setup.bash; 
echo '加载运行环境';
source /home/ft/ft_robot_ws/devel/setup.bash; 
roslaunch turn_on_wheeltec_robot navigation.launch 

