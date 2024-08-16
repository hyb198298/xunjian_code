#ifndef MY_UART
#define MY_UART

#include<iostream>
#include<thread>
#include<atomic>
#include <serial/serial.h>
#include"ros/ros.h"
#include"my_msg_pkg/sensor.h"
#include <bitset>
#include <sstream>
#include"my_msg_pkg/remot.h"
#include"my_msg_pkg/sensor_msg.h"
#include"my_msg_pkg/remot_msg.h"

class My_uart
{
private:
  ros::NodeHandle nh;
  serial::Serial serial;  // 串口对象  
  std::string port_name;  // 串口号
  int baud_rate;          // 波特率 
  std::string remot_rate; // 遥控器通讯协议
  std::string sensor_rate; // 传感器通讯协议
  std::vector<uint8_t> remot_data;  //遥控器字节
  std::vector<uint8_t> sensor_data; //传感器字节
  std::atomic<float> temperature;  //温度
  std::atomic<int> humidity;       //湿度
  std::atomic<float> smoke;        //烟雾水平
  std::atomic<int> noise;          //噪音
  ros::ServiceServer sensor_server; //传感器服务端
  ros::Publisher sensor_pub ;   //传感器发布者
  ros::Publisher remot_pub;  //遥控发布
  ros::ServiceClient   remot_client;//遥控客户端


public:
    My_uart();
    bool open_serial();
    std::vector<uint8_t> hexStrToBytes(const std::string& hex_str);
    void serial_work();
    bool serviceCallback(my_msg_pkg::sensorRequest&req,my_msg_pkg::sensorResponse&res);


    void get_sensor_data(const  std::vector<uint8_t>&  rev_data);
    void  get_remot_data( const std::vector<uint8_t> &rev_data);
    ~My_uart();
};


#endif


