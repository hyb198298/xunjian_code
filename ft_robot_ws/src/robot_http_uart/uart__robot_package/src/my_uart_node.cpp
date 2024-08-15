#include"my_uart.h"
int main(int argc,char*argv[])
{
   ros::init(argc,argv,"my_yart_node");
   My_uart my_uart;
   bool flag=my_uart.open_serial();
   if(!flag)
   {
      ROS_WARN("The serial port could not be opened,and the program exited!");
   };
   my_uart.serial_work();
   ros::shutdown();
   return 0;
}