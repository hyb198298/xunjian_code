#!/bin/bash

gnome-terminal --window  --title="ft_robot"  -e 'bash -c "/home/ft/ft_robot_ws/laser.sh;exec bash;"'\
 --tab --title="master_node"  -e 'bash -c "/home/ft/ft_robot_ws/uart.sh;exec bash;"'\
 --tab --title="master1_node"  -e 'bash -c "/home/ft/ft_robot_ws/base.sh;exec bash;"'\
 --tab --title="master2_node"  -e 'bash -c " /home/ft/ft_robot_ws/map_server.sh;exec bash"'\
 --tab --title="master3_node"  -e 'bash -c "/home/ft/ft_robot_ws/amcl.sh;exec bash;"'\
 --tab --title="master4_node"  -e 'bash -c "/home/ft/ft_robot_ws/nav.sh;exec bash;"'\
 --tab --title="master5_node"  -e 'bash -c "/home/ft/ft_robot_ws/master.sh;exec bash;"'\
 --tab --title="master6_node"  -e 'bash -c "/home/ft/ft_robot_ws/voice.sh;"'\
 --tab --title="master7_node"  -e 'bash -c "/home/ft/ft_robot_ws/http.sh;"'\
 --tab --title="master7_node"  -e 'bash -c "/home/ft/ft_robot_ws/point.sh;"'\



 