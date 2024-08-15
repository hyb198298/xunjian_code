#! /bin/bash
sleep 9;
echo 'uart环境'; 
source /opt/ros/melodic/setup.bash; 
echo '加载运行环境';
source /home/ft/ft_robot_ws/devel/setup.bash; 
 roslaunch ft_voice_package ft_voice.launch; 

