#!/bin/bash
sleep 7;
echo '加载环境'; 
source /opt/ros/melodic/setup.bash; 
echo '加载运行环境';
source /home/ft/ft_robot_ws/devel/setup.bash; 
mplayer /home/ft/ft_robot_ws/voice/巡检机器人.mp3;
roslaunch master_robot_package ft_master.launch 


