#ifndef  MASTER
#define MASTER

#include<iostream>
#include<thread>
#include<atomic>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <vector>
#include<cstdlib>
#include"ros/ros.h"
#include"my_msg_pkg/remot.h"
#include"my_msg_pkg/master.h"
#include"my_msg_pkg/remot_msg.h"
#include "std_msgs/String.h"
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2/LinearMath/Quaternion.h"
#include <tf2/LinearMath/Matrix3x3.h>
#include <mutex>
#include <condition_variable>
#include "move_base_msgs/MoveBaseAction.h"
#include "actionlib/client/simple_action_client.h"

enum RobotState {
  STOP_STATE = 0,  // 停止状态
  SIGN_STATE=4,//标记导航点状态
  REMOT_STATE = 3, // 遥控状态
  NAV_STATE = 2,    // 导航状态
  GMAP_STATE = 1    // 建图状态
};

class Master
{
    private:
    int Pre_state ;
    int Req_state;
    std::mutex mutx;             // 互斥锁
    std::string filename;
    ros::NodeHandle nh;
    // ros::ServiceServer  master_server;
    ros::Subscriber sub ;
    ros::Subscriber master_sub;
    ros::Publisher cmd_vel_pub;
    ros::Publisher chatter_pub;
    std::thread thread_cmd_vel;
    std::thread  thread_nav;
    std::atomic<bool>  contr_vel;
    std::atomic<bool>  nav_vel;
    std::atomic<bool> biao_dian;
    std::atomic<bool>  open_start;
    std::atomic<int>  select_vel;
    std::atomic<double>  pose_x;
    std::atomic<bool>gmapping_start;
    std::atomic<double>  pose_y;
    std::atomic<double>  pose_raw;
    
    std::condition_variable cv; // 条件变量
    std::vector<std::vector<double>> all_rout;

    
    
    void pub_vel();
    void pub_nav();


    public:
    Master();
    bool  get_request_state(int  req);
    ~Master();
    bool  Judge_state();
    // bool call_back(  my_msg_pkg::remotRequest & rep, my_msg_pkg::remotResponse &rsp);
    void  Call_back_sub(const my_msg_pkg::remot_msg   & req);
    void amclCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg);
    bool  Out_file(const double *data);
    bool  In_file(std::vector<std::vector<double>>& all_rout);
    bool  clear_file();
    bool  Judge_file();
    void nav_worker_thread();
};



#endif

