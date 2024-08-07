#include"my_uart.h"


My_uart::My_uart()
{
    temperature=0.0f;
    humidity=1;
    smoke=0.1f;
    noise=1;
    nh.getParam("/my_uart_node/port_name", port_name);
    nh.getParam("/my_uart_node/baud_rate", baud_rate);
    nh.getParam("/my_uart_node/remot_rate", remot_rate);
    nh.getParam("/my_uart_node/sensor_rate", sensor_rate);
    remot_data=hexStrToBytes(remot_rate);
    sensor_data=hexStrToBytes(sensor_rate);
    sensor_server=nh.advertiseService("get_sensor",&My_uart::serviceCallback, this);
    sensor_pub=nh.advertise<my_msg_pkg::sensor_msg>("sensor_topic", 10);
    remot_pub=nh.advertise<my_msg_pkg::remot_msg>("remot_topic", 10);
    remot_client=nh.serviceClient<my_msg_pkg::remot>("master_topic");

    for (uint8_t byte : remot_data) {  
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;

    for (uint8_t byte : sensor_data) {  
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
};

My_uart::~My_uart()
{
   
};

bool My_uart::open_serial()
{
  try
  {  
   serial.setPort("/dev/ttyS2");
   serial.setBaudrate(19200); 
   serial::Timeout _time = serial::Timeout::simpleTimeout(2000); 
    serial.setTimeout(_time);
    serial.open();
    if(serial.isOpen())
    {
      ROS_INFO("Serial and remote open serial successful.");
      return true; 
    }
    else
    {
      ROS_ERROR("Failed to open serial of sensor and remote port even after calling open().");
      return false;
     }
  }
  catch (const std::runtime_error& e)
  {
    ROS_ERROR("Cannot open serial port, please check the serial port cable. The reason is: %s", e.what());
    return false;
  }
};

void My_uart::serial_work()
{
   ros::Rate rate(1);
  while(ros::ok())
  {
    ros::spinOnce();
serial.write(remot_data);
usleep(20000); 
size_t length1 = serial.available();
std::cout<<"等代遥控器数据"<<std::endl;
std::cout << "Available data: " << length1 << std::endl;
if (length1 > 0) {
    std::vector<uint8_t> rev_data(length1);
    size_t bytesRead = serial.read(rev_data.data(), length1);
    if (bytesRead == length1 && rev_data[0]==0X08) {
      get_remot_data(rev_data);

       
    } else {
        std::cerr << "Error: Expected " << length1 << " bytes, but read " << bytesRead << " bytes." << std::endl;
    }
}
    usleep(200000); 
    serial.write(sensor_data);
    usleep(200000);
    size_t length2=serial.available();
    if(length2>0)
    {
          std::vector<uint8_t> rev_data(length2);
    size_t bytesRead = serial.read(rev_data.data(), length2);
    if (bytesRead == length2) {
        for (uint8_t byte : rev_data) {
            std::cout << std::hex << static_cast<int>(byte) << " ";
        }
        std::cout << std::dec << std::endl;
        if(rev_data[0]==0x01 && rev_data[1]==0x03 && rev_data[2]==0x16  )
        {
            get_sensor_data(rev_data);
            my_msg_pkg::sensor_msg msg;
            msg. temperature=temperature;
            msg. humidity= humidity;
            msg. noise=noise;
            msg.smoke=smoke;

            sensor_pub.publish(msg);

            std::cout << "温度: " << temperature<< ", 湿度: " << humidity
                      << ", 噪声: " <<noise << ", 烟雾: " <<smoke<< std::endl;
        }
        else
        {
             ROS_ERROR("this  sensor  head data  isn't   Expected! ");
        }
    }
     else 
     {
        ROS_ERROR("Error: Expected %d bytes, but read  %d  bytes.",length2,bytesRead);
    }
 
    }

  }
};

std::vector<uint8_t> My_uart::hexStrToBytes(const std::string& hex_str) {
    std::vector<uint8_t> bytes;
    std::size_t len = hex_str.length();
    std::istringstream iss(hex_str);
    std::string token;

    while (std::getline(iss, token, ' ')) { 
        std::size_t value;
        std::istringstream(token) >> std::hex >> value;
        bytes.push_back(static_cast<uint8_t>(value));
    }
    return bytes;
};

 bool My_uart::serviceCallback(my_msg_pkg::sensorRequest&req,my_msg_pkg::sensorResponse&res)
 {
       res.humidity=humidity;
       res.noise=noise;
       res.smoke=smoke;
       res.temperature=temperature;
       if(res.humidity==0 && res.noise==0 && res.smoke==0 && res.temperature==0)
       {
        ROS_ERROR("this dats of sensor is error!");
        return false;
       }
       return true;
 };
 
void My_uart:: get_sensor_data(const  std::vector<uint8_t>&  rev_data)
 {
            uint8_t temp_in00 = rev_data[3];
            uint8_t temp_in10 =rev_data[4];
            int temperature_raw = static_cast<int>(temp_in00) << 8 | temp_in10;
            float temperature = (temperature_raw - 2000) / 100.0;
            this->temperature=temperature;
            uint8_t hum_in00 =rev_data[5];
            uint8_t hum_in10 = rev_data[6];
            int humidity_raw = static_cast<int>(hum_in00) << 8 | hum_in10;
            int humidity = static_cast<int>(humidity_raw) / 100.0;
            this-> humidity= humidity;
            uint8_t noise_in00 = rev_data[13];
            uint8_t noise_in10 =rev_data[14];
            int noise_raw = static_cast<int>(noise_in00) << 8 | noise_in10;
            int noise = static_cast<int>(noise_raw) / 10.0;
            this->noise=noise;
            uint8_t smoke_in00 = rev_data[23];
            uint8_t smoke_in10 =rev_data[24];
            int smoke_raw = static_cast< int >(smoke_in00) << 8 | smoke_in10;
            float  smoke = static_cast< float >(smoke_raw) / 100.0;
            this->smoke=smoke;

 }

 void   My_uart::get_remot_data( const std::vector<uint8_t> &rev_data)
 {
   static   std::string  data="0000";


   std::string hex_str;
   std::string remot_vaul;
   my_msg_pkg::remot  client;
   my_msg_pkg::remot_msg    re_msg;
        for (uint8_t  byte : rev_data) {
            std::stringstream stream;
             stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
             hex_str+=stream.str();
        };
        if(hex_str.length() >= 10)
        {
          remot_vaul = hex_str.substr(6, 4);
          std::cout<<"按键的数值："<<remot_vaul<<std::endl;
          if(data==remot_vaul)
          {
            return ;
          }
          else{
            data=remot_vaul;
          }
          if(remot_vaul=="0000")
          {
            std::cout<<"遥控器正常，无操作"<<std::endl;
           
          }
          else if(remot_vaul=="0001")
          {
             
             re_msg.request=1;
            remot_pub.publish(re_msg);
             
            std::cout<<"前进"<<std::endl;
          }
           else if(remot_vaul=="0002")
          {
             re_msg.request=2;
            remot_pub.publish(re_msg);
            std::cout<<"后退"<<std::endl;
          }
          else if(remot_vaul=="0004")
          {
             re_msg.request=3;
            remot_pub.publish(re_msg);
            std::cout<<"左转"<<std::endl;
          }
          else if(remot_vaul=="0008")
          {
            std::cout<<"右转"<<std::endl;
             re_msg.request=4;
            remot_pub.publish(re_msg);
          }

            else if(remot_vaul=="0010")
          {
            std::cout<<"建图"<<std::endl;
             re_msg.request=5;
            remot_pub.publish(re_msg);
          }
           else if(remot_vaul=="0020")
          {
            std::cout<<"保存地图"<<std::endl;
             re_msg.request=6;
            remot_pub.publish(re_msg);
          }
          else if(remot_vaul=="0400")
          {
            std::cout<<"开始导航"<<std::endl;
            //插播语音播报
            re_msg.request=7;
            remot_pub.publish(re_msg);
          }
          else if(remot_vaul=="0040")
          {
            std::cout<<"closs mapping"<<std::endl;
            re_msg.request=13;
            remot_pub.publish(re_msg);
          }
           else if(remot_vaul=="0080")
          {
            re_msg.request=8;
            remot_pub.publish(re_msg);
             //插播语音播报
            std::cout<<"进入标记状态"<<std::endl;
          }
           else if(remot_vaul=="0100")
          {
             re_msg.request=9;
            remot_pub.publish(re_msg);
            std::cout<<"标记导航点"<<std::endl;
          }
          
            else if(remot_vaul=="0200")
          {
             re_msg.request=10;
            remot_pub.publish(re_msg);
            std::cout<<"删除导航点"<<std::endl;
          }

             else if(remot_vaul=="0800")
          {
            std::cout<<"停止"<<std::endl;
            re_msg.request=0;
            remot_pub.publish(re_msg);

          }
           else  if(remot_vaul=="0006")   
          {
            std::cout<<"后左"<<std::endl;
            re_msg.request=11;
            remot_pub.publish(re_msg);
          }
            else if(remot_vaul=="000a")
          {
             std::cout<<"后右"<<std::endl;
            re_msg.request=12;
            remot_pub.publish(re_msg);
          }
          else{
            ROS_ERROR("remot_vaul is  Error! please  check  this   remot data!");
          }
        }
        else
        {
          ROS_ERROR("this  data   of  remot  is error!");
        }
        
 }