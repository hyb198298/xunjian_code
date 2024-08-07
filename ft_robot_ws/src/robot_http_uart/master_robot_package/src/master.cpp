
#include"master.h"

 Master::~Master()
 {
    select_vel=0;
     contr_vel=false;
     nav_vel=false;
     open_start=false;
  
        if ( thread_cmd_vel.joinable()) {
              thread_cmd_vel.join();
        }
        if ( thread_nav.joinable()) {
              thread_nav.join();
        }
 }
Master::Master()
{
     Pre_state=0;
     Req_state=-1;
     filename = "/home/ft/ft_robot_ws/src/robot_http_uart/master_robot_package/config/ft_route.txt";

     sub= nh.subscribe("amcl_pose", 10, &Master::amclCallback,this);
    // master_server=nh.advertiseService("master_topic", &Master::call_back, this);
    master_sub = nh.subscribe("remot_topic", 10, &Master::Call_back_sub,this);
    cmd_vel_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    chatter_pub=nh.advertise<std_msgs::String>("chatter", 10);
    thread_cmd_vel = std::thread(&Master:: pub_vel,this);
    thread_nav=std::thread(&Master::nav_worker_thread,this);
     select_vel=0;
     contr_vel=true;
     gmapping_start=false;
     nav_vel=false;
     biao_dian=false;
     open_start=true;
    
}


void Master::Call_back_sub(const my_msg_pkg::remot_msg   & req)
{
    int  new_state=0;
     std::cout<<"会调相应"<<std::endl;//
      std_msgs::String msg;
        
    if(0<=req.request  &&  req.request<=13)
    {
        get_request_state(req.request);
        std::cout<<req.request<<"       "<<Req_state<<std::endl;

        if(!Judge_state())
        {
            ROS_ERROR("this request  state   is  error,please check!  ");
            std::cout<<"炒作失败"<<"当前状态"<<Pre_state<<"   请求状态"<<Req_state<<std::endl;//
            msg.data = "功能切换失败.mp3";
            chatter_pub.publish(msg);
            return  ;
        }
        std::cout<<"可以切换"<<"当前状态"<<Pre_state<<"   请求状态"<<Req_state<<std::endl;
        if(Req_state==REMOT_STATE)     
        {
            if(Pre_state==STOP_STATE)   
            {
                Pre_state=REMOT_STATE;
            }
            contr_vel=true;
            select_vel=req.request;
            std::cout<<"发布速度指令"<<std::endl;
           
        }
        else  if(Req_state==GMAP_STATE)  
        {
                    if(Pre_state==GMAP_STATE) 
                    {
                        if(req.request==5)    
                        {
                            //ROS_ERROR(" this    GMAP_STATE  has   open!");
                            msg.data = "重复建图.mp3";
                            chatter_pub.publish(msg);
                            std::cout<<"重复建图"<<std::endl;
                        }
                        else  if( req.request==6) 
                        {
                            std::cout<<"保存地图"<<std::endl;
                            // system("/bin/bash -c '/home/ft/ft_robot_ws/map_sav.sh'");
                            // //  int result=  system(" source /home/ft/ft_robot_ws/map_sav.sh");
                            //  int result = system("/bin/bash /home/ft/ft_robot_ws/map_sav.sh");
                            int result= system("/bin/bash -c 'source /home/ft/ft_robot_ws/map_sav.sh'");
                             std::cout<<result<<std::endl;
                             sleep(10);
                            if(result==0)
                            {
                                sleep(2);
                                msg.data = "地图保存成功.mp3";
                                chatter_pub.publish(msg);
                                
                                system("  rosnode kill /map_save");
                            }
                            else{
                                std::cout << "地图失败" << std::endl;
                                msg.data = "地图保存失败.mp3";
                                chatter_pub.publish(msg);
                                system("  rosnode kill /map_save");
                            }
                        }
                        else  
                        {
                              int result= system("/bin/bash -c 'rosnode kill /slam_gmapping '");
                              
                            if(result==0)
                            {
                                msg.data = "建图关闭成功.mp3";
                                chatter_pub.publish(msg);
                            }
                            else
                            {
                                std::cout << "地图失败" << std::endl;
                                msg.data = "建图关闭失败.mp3";
                                chatter_pub.publish(msg);
                            }
                            gmapping_start=false;
                            // int result= system("source  /home/ft/ft_robot_ws/amcl.sh ");
                             int resuleeet= system("/bin/bash -c 'source /home/ft/ft_robot_ws/my_amcl.sh'");
                            //  int resulwwt = system("/home/ft/ft_robot_ws/map_sav.sh");
                            std::cout<<result<<std::endl;
                             sleep(10);
                        }
                    }
                    else
                    {
                        
                        if(req.request==5)    
                        {
                            Pre_state=GMAP_STATE;
                         
                            std::cout<<"建图启动中"<<std::endl;
                            int result= system("/bin/bash -c 'source /home/ft/ft_robot_ws/gmapping.sh'");
                            if(result==0)
                            {
                                std::cout<<"地图成功"<<std::endl;
                                 msg.data = "建图开启成功.mp3";
                                chatter_pub.publish(msg);
                                gmapping_start=true;
                            }
                            else{
                                std::cout<<"地图失败"<<std::endl;
                                 msg.data = "建图开启失败.mp3";
                                chatter_pub.publish(msg);
                            }
                        }
                        else   
                        {
                            std::cout<<"建图未启动无法保存地图"<<std::endl;
                        }
                    }
        }
        else if(Req_state==NAV_STATE)
        {
            if(Pre_state==NAV_STATE)
            {
                std::cout << "重复导航" << std::endl;
                msg.data = "导航重复启动.mp3";
                chatter_pub.publish(msg);
            }
            else
            {
                Pre_state=NAV_STATE;
                std::cout<<"开启导航"<<std::endl;
                std::vector<std::vector<double>> rout;
                if(In_file(rout))
                {

                    all_rout=rout;
                    select_vel=req.request;
                    nav_vel=true;
       

                   std::cout<<"播放导航工作"<<std::endl;
                   sleep(5);
                    msg.data = "导航开启成功.mp3";
                    chatter_pub.publish(msg);
                }
                else
                {
                     msg.data = "导航功能开启失败.mp3";
                    chatter_pub.publish(msg);
                };

            }
        }
        else if(Req_state==STOP_STATE)   
        {
            std::cout<<"停止一切活动"<<std::endl;
            // system("rostopic pub /move_base/cancel actionlib_msgs/GoalID -- {}  ");
            if(nav_vel==true)
            {
                 ros::Publisher pub = nh.advertise<actionlib_msgs::GoalID>("/move_base/cancel", 10);
                actionlib_msgs::GoalID nav_msg;
                pub.publish(nav_msg);
                std::cout << "关闭导航" << std::endl;
                std::cout<<"播放导航工作"<<std::endl;
                   sleep(5);
                msg.data = "导航关闭.mp3";
          
                chatter_pub.publish(msg);
            }
                  biao_dian=false;
              nav_vel=false;
             contr_vel=true;
            select_vel=0;  
            if(gmapping_start==false)
            {
                 Pre_state=STOP_STATE;
            }
    
        }
        else  if(Req_state==SIGN_STATE)  //标点
        {
            
                 Pre_state=SIGN_STATE;
                 biao_dian=true;
                 if(req.request==9)
                {
                     double  data[3];
                     data[0]=pose_x;
                     data[1]=pose_y;
                     data[2]=pose_raw;
                     std::cout<<"data:"<<data[0]<<"------------"<<data[1]<<" -------"<<data[2]<<std::endl;
                    if(Out_file(data))
                    {
                        std::cout<<"导航点标记成功"<<std::endl; 
                        msg.data = "导航点标记成功.mp3";
                        chatter_pub.publish(msg);

                    }
                    else{
                        std::cout<<"导航点标记失败"<<std::endl;
                        msg.data = "导航点标记失败.mp3";
                        chatter_pub.publish(msg);

                    } 
                }
                else   
                {
                    if(Judge_file())
                    {    
                         if( clear_file())
                        {
                          std::cout<<"删除导航点成功"<<std::endl;
                          msg.data = "导航点清除.mp3";
                          chatter_pub.publish(msg);
                        }
                       else{
                          std::cout<<"导航点清除失败"<<std::endl;
                          msg.data = "导航点清除失败.mp3";
                          chatter_pub.publish(msg);
                        } 
                   }   
                   else{
                       std::cout<<"文件为空"<<std::endl;

                   }     
                }
         }
        else{
            std::cout<<"指令错误"<<std::endl;
        }
    
    }
}





bool  Master::get_request_state(int req)
{
    if(req==1 ||   req==2   || req==3 ||  req==4  || req==11  || req==12)  
        {
           Req_state=REMOT_STATE;
        }
        else if(req==5  ||  req==6 || req==13 ) 
        {
            Req_state=GMAP_STATE;
        }
        else if(req==7) //导航
        {
           Req_state=NAV_STATE;
        }
        else  if(req==8  ||  req==9   ||req==10) 
        {
           Req_state=SIGN_STATE;
        }
        else if(req==0)   
        {
            Req_state=STOP_STATE;
        }
        else
        {
            ROS_WARN("this   req_state  is   error! please   check!");
            return   false;
        }
        return true;
};

bool   Master::Judge_state()
{
     std::cout<<"Judge_state"<<std::endl;//
    switch (Req_state)
    {
        case REMOT_STATE:
        {
    
            if (Pre_state == NAV_STATE)
            {
               
                return false; 
            }  
            break;
        }

        case GMAP_STATE:
        {
          
            if (Pre_state != STOP_STATE && Pre_state != GMAP_STATE)
           {
                return false; 
            }  
            break;
        }

        case NAV_STATE:
        {
            if (Pre_state != STOP_STATE )
            {
                return false; 
            }  
            break;
        }

        case SIGN_STATE:
        {
            if (Pre_state != STOP_STATE && Pre_state != SIGN_STATE)
            {
               return false; 
            } 
            break;
        }
        case STOP_STATE:
            break;

        default:
        {

                return false; 
         }  
   }
   return true;
};

bool    Master:: Out_file(const double *data)
 {
         std::ofstream outfile(filename, std::ios::app); 

         if (!outfile.is_open()) {
              std::cerr << "无法打开文件: " << filename << std::endl;
              return  false;
           }
        for (size_t i = 0; i < 3; ++i) {
            outfile << data[i];
            if (i < 2) {
                outfile << " "; 
            }
        }
        outfile << std::endl; 
        outfile.close();

         std::cout << "数据追加写入成功。" << std::endl;
};
bool    Master:: In_file(  std::vector<std::vector<double>> &all_rout)
{
     
     if(!Judge_file())
     {
        return false;
     }

     std::ifstream infile(filename);

    if (!infile.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        infile.close();
        return false;
    }
    std::string line;

    while (std::getline(infile, line)) {
        std::vector<double> numbers; 
        std::istringstream iss(line);

        double number;
        while (iss >> number) {
            numbers.push_back(number); 
        }

        all_rout.push_back(numbers); 
    }

    infile.close();
    for (const auto& numbers : all_rout) {
        for (const auto num : numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
  std::cout << "文件读取并提取数字完成。" << std::endl;
  return  true;

 };
 bool     Master::clear_file()
 {
     std::ofstream clearfile(filename, std::ios::out | std::ios::trunc);
     if (!clearfile.is_open()) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return false;
    }
    clearfile.close();
    std::cout << "文件内容已被删除，但文件名保留。" << std::endl;
    return  true;

 };
bool     Master::Judge_file()
 {
     std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "错误：无法打开文件" << std::endl;
        return false; 
    }
    file.seekg(0, std::ios::beg);
    char ch = file.get(); 
    if (file.eof()) {
        std::cout << "文件没有数据。" << std::endl;
    } else {
        file.seekg(0, std::ios::beg);
        std::cout << "文件包含数据。" << std::endl;
        file.close();
        return true;
    }
    file.close();
    return  false;
};


#define  SPD 0.5
void Master:: pub_vel()
{
    while( contr_vel)//停止按钮
    {
        // std::cout << "速度控制-----" << std::endl;
        usleep(200000);
        geometry_msgs::Twist twist;
        double  vel=0.0;
        if(gmapping_start==true || biao_dian==true)
        {
            vel=0.2;
        }
        else
        {
            vel=0.4;
        }
        if(select_vel==1)  //前进
        {
            // std::cout << "前进-----" << std::endl;
            twist.linear.x = vel;  // 线速度，沿x轴，单位：米/秒
            twist.angular.z = 0.0; // 角速度，绕z轴，单位：弧度/秒
            cmd_vel_pub.publish(twist);
        }
        else   if(select_vel==2)  //后退
        {
            // std::cout << "后退-----" << std::endl;
            twist.linear.x = -vel;  // 线速度，沿x轴，单位：米/秒
            twist.angular.z = 0.0; // 角速度，绕z轴，单位：弧度/秒
            cmd_vel_pub.publish(twist);
        }
        else   if (select_vel==3)//左
        {
            //  std::cout << "左-----" << std::endl;
             twist.linear.x = vel;  // 线速度，沿x轴，单位：米/秒
            twist.angular.z = vel/1.43; // 角速度，绕z轴，单位：弧度/秒
            cmd_vel_pub.publish(twist);
        }
        else  if(select_vel==4) //右
        {
            //  std::cout << "右-----" << std::endl;
             twist.linear.x = vel;  // 线速度，沿x轴，单位：米/秒
            twist.angular.z =- vel/1.43;// 角速度，绕z轴，单位：弧度/秒
            cmd_vel_pub.publish(twist);
        }
           else  if(select_vel==11) //左后
        {
            //  std::cout << "左后-----" << std::endl;
             twist.linear.x = -vel;  // 线速度，沿x轴，单位：米/秒
            twist.angular.z =- vel/1.43;// 角速度，绕z轴，单位：弧度/秒
            cmd_vel_pub.publish(twist);
        }
         else  if(select_vel==12) //右后
        {
             twist.linear.x = -vel;  // 线速度，沿x轴，单位：米/秒
            twist.angular.z = vel/1.43;// 角速度，绕z轴，单位：弧度/秒
            cmd_vel_pub.publish(twist);
        }
        else if(select_vel==0) //停止
        {  
          twist.linear.x = 0.0;  // 线速度，沿x轴，单位：米/秒
          twist.angular.z = 0.0 ;// 角速度，绕z轴，单位：弧度/秒
          cmd_vel_pub.publish(twist);
        }
        else if(select_vel==7) //停止
        {   
       
        }
        else
        {
             twist.linear.x = 0.0;
             twist.angular.z = 0.0 ;
             cmd_vel_pub.publish(twist);
        }
        
    }

};

void Master::pub_nav()
{
    ros::Rate rate(100);
    std::vector<std::vector<double>> all_rout;
    // if(!Out_file(all_rout))
    // {
    //     std::cout<<"导航点无数据"<<std::endl;
    //     return  ;
    // }
    while(nav_vel)
    {
        rate.sleep();
        std::cout<<"导航进行中"<<std::endl;
    }
}
void Master::amclCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg) {

    pose_x= msg->pose.pose.position.x;
    pose_y= msg->pose.pose.position.y;

    tf2::Quaternion qtn(msg->pose.pose.orientation.x,
                         msg->pose.pose.orientation.y,
                         msg->pose.pose.orientation.z,
                         msg->pose.pose.orientation.w);
    ROS_INFO("Quaternion x: %f, y: %f, z: %f, w: %f",
            msg->pose.pose.orientation.x,
                         msg->pose.pose.orientation.y,
                         msg->pose.pose.orientation.z,
                         msg->pose.pose.orientation.w);
    double roll, pitch, yaw;
    tf2::Matrix3x3(qtn).getRPY(roll, pitch, yaw);

    pose_raw=yaw;
    std::cout<<"yaw:"<<yaw<<std::endl;
    qtn.setRPY(0,0,yaw);
    std::cout<<"pose :"<< pose_x<<"        "<< pose_y<<"         四元素 :  "<<qtn.getX()<<"  "<<qtn.getY()<<","<<qtn.getZ()<<","<<qtn.getW()<<std::endl;

        
}

void Master::nav_worker_thread()
 {
    while (open_start) {
        int size=0;
        if (nav_vel) {
             std::unique_lock<std::mutex> lock(mutx); 
       
            actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base", true);
            for (auto& vect : all_rout) {
                
                std::cout << vect[0] << " " << vect[1] << " " << vect[2] << std::endl;
                actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>  ac("move_base", true);
                 ROS_INFO("Waiting for the move_base action server to start.");
                 if (!ac.waitForServer(ros::Duration(5.0)))
                 {
                     ROS_ERROR("Server not available!");
                     return;
                 }
                 ROS_INFO("Server started!");
                 move_base_msgs::MoveBaseGoal goal;
                 goal.target_pose.header.frame_id = "map";
                 goal.target_pose.header.stamp = ros::Time::now();
                 goal.target_pose.pose.position.x = vect[0];
                 goal.target_pose.pose.position.y = vect[1];
                 goal.target_pose.pose.position.z = 0.0;
                 tf2::Quaternion qtn;
                 qtn.setRPY(0, 0, vect[2]);
                 goal.target_pose.pose.orientation.x = qtn.getX();
                 goal.target_pose.pose.orientation.y = qtn.getY();
                 goal.target_pose.pose.orientation.z = qtn.getZ();
                 goal.target_pose.pose.orientation.w = qtn.getW(); // 四元数
                 ROS_INFO("Sending goal to move_base action server.");
                 ac.sendGoal(goal);
                 std_msgs::String msg;


               
                 msg.data = "巡检进行中.mp3";
                 chatter_pub.publish(msg);

                 while (true)
                 {
                     if (std::abs(pose_x - vect[0]) < 0.5 && std::abs(pose_y - vect[1]) < 0.5)
                     {
                         break; 
                         size+=1;
                         if(size==all_rout.size()+1)
                         {
                            nav_vel=false;
                         }
                         std::cout << "到达目标*************************" << std::endl;
                     }
                     std::this_thread::sleep_for(std::chrono::milliseconds(500));
                     if (!nav_vel)
                      {
                        break;

                      }
                }
                if(!nav_vel)
                {
                    break;
                }

            
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); 

    }
}









































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































// /------------------------------------------
// bool Master::call_back( my_msg_pkg::remotRequest& req, my_msg_pkg::remotResponse& rsp)
// {
//     int  new_state=0;
//     if(0<=req.request  &&  req.request<=10)
//     {
//         get_request_state(req.request);
//         //--------------------------------------------------------------------------------------------------------状态机切换判断
//         if(!Judge_state())
//         {
//             ROS_ERROR("this request  state   is  error,please check!  ");
//             std::cout<<"炒作失败"<<"当前状态"<<Pre_state<<"   请求状态"<<Req_state<<std::endl;
//             rsp.response=0;
//             return  false;
//         }
//         //——————————————————————————————————————————————————————————开始切换
//         std::cout<<"可以切换"<<"当前状态"<<Pre_state<<"   请求状态"<<Req_state<<std::endl;
//         bool    flag;
//         if(Req_state==REMOT_STATE)     //要求进行遥控操作
//         {
//             if(Pre_state==STOP_STATE)   //遥控态只在停止太切换
//             {
//                 Pre_state=REMOT_STATE;
//                 flag=true;
//             }
//             //解析数据发布
//             contr_vel=true;
//             select_vel=req.request;
//             std::cout<<"发布速度指令"<<std::endl;
           
//         }
//         else  if(Req_state==GMAP_STATE)  //建图
//         {
//                     if(Pre_state==GMAP_STATE) //建图已经启动
//                     {
//                         if(req.request==5)    //重复建图
//                         {
//                             //ROS_ERROR(" this    GMAP_STATE  has   open!");
//                             std::cout<<"重复建图"<<std::endl;
//                             flag=false;
//                         }
//                         else   //保存地图
//                         {
//                             std::cout<<"保存地图"<<std::endl;
//                             flag=true;
//                         }
//                     }
//                     else//建图未启动
//                     {
                        
//                         if(req.request==5)    //建图
//                         {
//                             Pre_state=GMAP_STATE;
//                             std::cout<<"建图启动中"<<std::endl;
//                              flag=true;
//                         }
//                         else   //保存地图
//                         {
//                             std::cout<<"建图未启动无法保存地图"<<std::endl;
//                             flag=false;
//                         }
//                     }
//         }
//         else if(Req_state==NAV_STATE)//导航
//         {
//             if(Pre_state==NAV_STATE)
//             {
//                 std::cout<<"重复导航"<<std::endl;
//                   flag=false;
//             }
//             else
//             {
//                 Pre_state=NAV_STATE;
//                 std::cout<<"开启导航"<<std::endl;
//                 flag=true;
//             }
//         }
//         else if(Req_state==STOP_STATE)   //停止
//         {
//             std::cout<<"停止一切活动"<<std::endl;
//             select_vel=0;     //发布速度为0
//             //关闭见图
//             //关闭导航
//              nav_vel=false;
//              //发布move_base指令

//             Pre_state=STOP_STATE;
//         }
//         else  if(Req_state==SIGN_STATE)  //标点
//         {
//             if(Pre_state==SIGN_STATE)
//             {
//                 if(req.request==8)
//                 {
//                  std::cout<<"重复标点"<<std::endl;
//                  flag=false;
//                 }
//                 else   if(req.request==9)
//                 {
//                      double  data[3];
//                     if(Out_file(data))
//                     {
//                         std::cout<<"导航点标记成功"<<std::endl;
//                         flag=true;
//                     }
//                     else{
//                         std::cout<<"导航点标记失败"<<std::endl;
//                         flag=false;
//                     } 
//                 }
//                 else   
//                 {
//                     if(Judge_file())
//                     {    
//                          if( clear_file())
//                         {
//                           std::cout<<"删除导航点成功"<<std::endl;
//                          flag=true;
//                         }
//                        else{
//                           std::cout<<"删除导航点失败"<<std::endl;
//                           flag=false;
//                         } 
//                    }   
//                    else{
//                        std::cout<<"文件为空"<<std::endl;
//                    }     
//                 }
//             }
//             else
//             {
//                 if(req.request==8)
//                 {
//                    std::cout<<"标点启动中"<<std::endl;
//                     Pre_state=SIGN_STATE;
//                     flag=true;
//                 }
//                 else
//                 {
//                      std::cout<<"标点服务未启动"<<std::endl;
//                       flag=false;
//                 }
//             }
//         }
//         else{
//             flag=false;
//         }
//         if(!flag)
//         {
//             rsp.response=0;
//             return false;
//         }
//         else{
//              rsp.response=1;
//             return true;
//         }
//     }
//     else
//     {
//         ROS_ERROR("this remotRequest is error  !");
//         return  false;
//     }
// };

//  ros::Publisher pub = nh.advertise<actionlib_msgs::GoalID>("/move_base/cancel", 10);
//                 actionlib_msgs::GoalID nav_msg;
//                 pub.publish(nav_msg);




//   actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>  ac("move_base", true);

//                  // 等待直到action服务器准备好
//                  ROS_INFO("Waiting for the move_base action server to start.");
//                  if (!ac.waitForServer(ros::Duration(5.0)))
//                  {
//                      ROS_ERROR("Server not available!");
//                      return;
//                  }
//                  ROS_INFO("Server started!");

//                     // 创建目标Pose消息
                 
//                  move_base_msgs::MoveBaseGoal goal;
//                  goal.target_pose.header.frame_id = "map";
//                  goal.target_pose.header.stamp = ros::Time::now();
//                  goal.target_pose.pose.position.x = vect[0];
//                  goal.target_pose.pose.position.y = vect[1];
//                  goal.target_pose.pose.position.z = 0.0; // 通常设置为0

//                  // 发布导航目标
//                  tf2::Quaternion qtn;
//                  qtn.setRPY(0, 0, vect[2]);
//                  goal.target_pose.pose.orientation.x = qtn.getX();
//                  goal.target_pose.pose.orientation.y = qtn.getY();
//                  goal.target_pose.pose.orientation.z = qtn.getZ();
//                  goal.target_pose.pose.orientation.w = qtn.getW(); // 四元数表示的旋转
//                  ROS_INFO("Sending goal to move_base action server.");
//                  ac.sendGoal(goal);
//                  std_msgs::String msg;