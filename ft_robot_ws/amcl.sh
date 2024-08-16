#! /bin/bash
sleep 4;
echo 'amcl  启动'; 
source /opt/ros/melodic/setup.bash; 
echo '加载运行环境';
source /home/ft/ft_robot_ws/devel/setup.bash; 

roslaunch amcl ft_amcl.launch;
exec bash;