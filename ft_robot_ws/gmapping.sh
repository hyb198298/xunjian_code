#!/bin/bash
gnome-terminal --window --title="ft_gmapping" -- bash -c "
echo '加载环境';
source /opt/ros/melodic/setup.bash;

source /home/ft/ft_robot_ws/devel/setup.bash;
echo 'amcl  以杀除';
rosnode kill /amcl;

sleep  0.1;

roslaunch gmapping ft_gamapping.launch;


sleep  0.1;
echo 'amcl  以开启';



"
