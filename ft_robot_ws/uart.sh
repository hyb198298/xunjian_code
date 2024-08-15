#! /bin/bash
sleep 3;
echo 'uart环境'; 
source /opt/ros/melodic/setup.bash; 
echo '加载运行环境';
source /home/ft/ft_robot_ws/devel/setup.bash; 
roslaunch uart__robot_package my_uart_node.launch
