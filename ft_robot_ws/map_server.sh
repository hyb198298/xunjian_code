sleep 3;
echo 'map_server 启动'; 
source /opt/ros/melodic/setup.bash; 
echo '加载运行环境';
source /home/ft/ft_robot_ws/devel/setup.bash; 


roslaunch map_server ft_server_map.launch;
exec bash;