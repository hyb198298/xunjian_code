#include "httplib.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "ros/ros.h"
#include <atomic>
#include <signal.h>
#include "my_msg_pkg/sensor_msg.h"
#include "my_msg_pkg/remot_msg.h"
#include <thread> // 包含线程支持

using namespace httplib;
std::mutex mtx; // 用于同步的互斥体
std::atomic<bool> stop_signal_received(false);

struct SensorData {
    std::atomic<float> temperature;
    std::atomic<int> humidity;
    std::atomic<float> smoke;
    std::atomic<int> noise;
};



class HttpServer {
private:
    std::atomic<bool> stop_time;
    std::string remot_topic;
    Server svr;
    std::thread server_thread;
    std::thread time_thread;
     ros::NodeHandle nh;
    ros::Publisher remot_pub;
    ros::Subscriber sensor_sub;
    SensorData data;
    std::vector<std::tm>my_time;
public:
    void pub_Ontime()
    {
        while (stop_time)
        {
            sleep(30);
            std::lock_guard<std::mutex> lock(mtx);
            auto now = std::chrono::system_clock::now();
            auto now_c = std::chrono::system_clock::to_time_t(now);
            std::tm now_tm = *std::localtime(&now_c);

            std::cout << "发布导航数据11111222" << std::endl;
            for (const auto &time_open : my_time)
            {
                std::cout << "发布导航数据heming222" << std::endl;

                
                // 检查当前时间是否在发布时间前后2秒内
                std::cout << time_open.tm_hour<<"    "<<time_open.tm_min<<""<<time_open.tm_sec<< std::endl;
                std::cout << now_tm.tm_hour<<"    "<<time_open.tm_min<<""<<time_open.tm_sec<< std::endl;
                if ((time_open.tm_hour-now_tm.tm_hour)==0  && (now_tm.tm_min-time_open.tm_min<1) &&  (now_tm.tm_min-time_open.tm_min>0))
                {
                    my_msg_pkg::remot_msg re_msg;
                    re_msg.request = 7;  
                    remot_pub.publish(re_msg);
                    std::cout << "发布导航数据" << std::endl;
                }
            }
        }
    }
    // 将std::tm转换为ros::Time的辅助函数
    ros::Time tm_to_rostime(const std::tm &time_struct)
    {

       auto shared_tm= std::make_shared<std::tm>(time_struct);
         std::cout << "发布导航数据1123" << std::endl;

        std::time_t time_t_value = std::mktime(const_cast<std::tm *>(shared_tm.get()));
                            std::cout << "发布导航数据5678" << std::endl;



                             ros::Time  now_ros (time_t_value);

        return   now_ros;
    }

    HttpServer() {
        stop_time=true;
        remot_pub = nh.advertise<my_msg_pkg::remot_msg>("remot_topic", 10);
        sensor_sub = nh.subscribe("sensor_topic", 10, &HttpServer::sensorCallback, this);
        time_thread=std::thread(&HttpServer::pub_Ontime, this);
        initHttpServer();
    }
    ~HttpServer()
    {
        stop_time=false;
        
        if (time_thread.joinable())
        {
            time_thread.join();
        }
    }

    void sensorCallback(const my_msg_pkg::sensor_msg::ConstPtr &msg) {
        ROS_INFO("传感器数据以接受！");
        data.temperature.store(msg->temperature);
        data.humidity.store(msg->humidity);
        data.smoke.store(msg->smoke);
        data.noise.store(msg->noise);
    }

    std::string sensor_data_to_json(const SensorData &data) {
        std::string json = R"({"temperature":)" + std::to_string(data.temperature.load()) +
                           R"(,"humidity":)" + std::to_string(data.humidity.load()) +
                           R"(,"smoke":)" + std::to_string(data.smoke.load()) +
                           R"(,"noise":)" + std::to_string(data.noise.load()) +
                           "}";
        return json;
    }

    void initHttpServer() {
        svr.Get("/get/sensor", [this](const Request &req, Response &res) {
            std::cout << "传感器数据请求以接受" << std::endl;
            std::string json = sensor_data_to_json(data);
            set_response_header(res);
            res.set_content(json, "application/json");
        });

        svr.Get("/robot/inspect/start", [this](const Request &req, Response &res) {
            std::cout << "开始巡检请求已接受" << std::endl;
            my_msg_pkg::remot_msg re_msg;
            re_msg.request = 7;
            remot_pub.publish(re_msg);
            set_response_header(res);
            res.set_content(R"({"result": "1"})", "application/json");
        });

        svr.Get("/robot/inspect/stop", [this](const Request &req, Response &res) {
            std::cout << "停止巡检请求以接受" << std::endl;
            my_msg_pkg::remot_msg re_msg;
            re_msg.request = 0;
            remot_pub.publish(re_msg);
            set_response_header(res);
            res.set_content(R"({"result":"1"})", "application/json");
        });

        svr.Get("/robot/inspect/state", [this](const Request &req, Response &res) {
            std::cout << "巡检状态请求已被接受" << std::endl;
            set_response_header(res);
            res.set_content(R"({"result":"1","robotid":"Ft123456"})", "application/json");
        });

        svr.Get("/robot/inspect/timeSet", [this](const Request &req, Response &res) {
            std::cout << "定时巡检设置请求已接受" << std::endl;
            std::vector<std::string>  starttime;
           starttime.push_back( req.get_param_value("start1time"));
            starttime.push_back( req.get_param_value("start2time"));
            starttime.push_back( req.get_param_value("start3time"));
            std::tm time;
            std::lock_guard<std::mutex> lock(mtx);
            for(int i=0;i<3;i++)
            {
                if(starttime[i]=="")
                {
                    std::cout<<"this time is null"<<std::endl;
                }
                else{

                    std::stringstream ss(starttime[i]);

                    int hours, minutes;
                    char colon; 

                    ss >> hours >> colon >> minutes;

                    // 检查是否成功读取并打印结果
                    if (ss.fail() || colon != ':')
                    {
                        std::cerr << "heming 时间字符串格式不正确" << std::endl;
                    }
                    else
                    {
                        std::cout << "heming 小时: " << hours << " 分钟: " << minutes << std::endl;
                    }

                    
                    time.tm_hour=hours;
                    time.tm_min=minutes;
                    time.tm_sec=0;
                   my_time.push_back(time);
                }
            }
            set_response_header(res);
            res.set_content(R"({"result":"1"})", "application/json");
        });
    }

    void startServerThread() {
        server_thread = std::thread([this]() {
            std::cout << "接口监听启动中" << std::endl;
            auto ret = svr.listen("0.0.0.0", 5001);
            if (ret == 0) {
                std::cerr << "服务器启动失败...\n";
                exit(-1);
            }
        });
    }

    void stopServerThread() {
        svr.stop();
        std::cout<<"http has  stop!"<<std::endl;
        if (server_thread.joinable()) {
            server_thread.join();
        }
    }

private:
    void set_response_header(Response &res) {
        res.set_header("Content-Type", "application/json");
        res.status = 200;
    }
};

void signal_handler(int sig) {
    std::cout << "接收到中断信号，正在关闭服务器...\n";
    stop_signal_received.store(true);
    ros::shutdown();
}

int main(int argc, char *argv[]) {



    std::cout<<"网络服务打开"<<std::endl;
    signal(SIGINT, signal_handler);
    setlocale(LC_ALL, "");
    ros::init(argc, argv, "my_ros_http_node");

    HttpServer node;
    node.startServerThread(); // 启动HTTP服务器线程

    while (ros::ok()) {
        ros::spinOnce();
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 避免占用过多CPU资源
    }

    node.stopServerThread(); // 停止HTTP服务器线程
    return 0;
}