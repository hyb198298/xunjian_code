#!/bin/bash
gnome-terminal --window --title="ft_gmapping" -- bash -c "
echo '加载环境';
source /opt/ros/melodic/setup.bash;
source /home/ft/ft_robot_ws/devel/setup.bash;
roslaunch amcl ft_amcl.launch;
sleep  0.1;
echo 'amcl  以开启';



"