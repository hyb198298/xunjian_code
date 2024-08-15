#! /bin/bash
sleep 3.5;
echo 'http'; 
source /opt/ros/melodic/setup.bash; 
echo '加载运行环境';
source /home/ft/ft_robot_ws/devel/setup.bash; 
roslaunch my_ros_http_node http.launch