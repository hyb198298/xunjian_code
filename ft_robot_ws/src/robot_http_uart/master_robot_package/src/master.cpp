
#include"master.h"

 Master::~Master()
 {
    select_vel=0;
     contr_vel=false;
     nav_vel=false;
     open_start=false;
     biao_dian=false;
  
        if ( thread_cmd_vel.joinable()) {
              thread_cmd_vel.join();
        }
        if ( thread_nav.joinable()) {
              thread_nav.join();
        }
 }
Master::Master()
{
    select_vel = 0;
    contr_vel = true;
    open_point = false;
    gmapping_start = false;
    nav_vel = false;
    biao_dian = false;
    open_start = true;
    Pre_state = 0;
    Req_state = -1;
    filename = "/home/ft/ft_robot_ws/src/robot_http_uart/master_robot_package/config/ft_route.txt";
    data[0]=0.0;
    data[1]=0.0;
    data[2]=0.0;
    open_luzhi=false;
    ver_control=0.0;
    bool_raw=false;

    
    sub = nh.subscribe("amcl_pose", 10, &Master::amclCallback, this);
    pre_point_pub= nh.advertise<geometry_msgs::PointStamped>("pre_point", 1, true);
    min_point_pub= nh.advertise<geometry_msgs::PointStamped>("min_point", 1, true);
    // master_server=nh.advertiseService("master_topic", &Master::call_back, this);
    master_sub = nh.subscribe("remot_topic", 1000, &Master::Call_back_sub,this);
    point_sub=nh.subscribe("int_point",10,&Master::point_Call_back,this);
    cmd_vel_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    chatter_pub=nh.advertise<std_msgs::String>("chatter", 10);
    thread_cmd_vel = std::thread(&Master:: pub_vel,this);
    path_pub = nh.advertise<nav_msgs::Path>("trajectory", 1, true);
    path_pubr = nh.advertise<nav_msgs::Path>("trajectortttty", 1, true);
    B_path_pub = nh.advertise<nav_msgs::Path>("nussb_path", 1, true);
    thread_nav=std::thread(&Master::nav_worker_thread,this);
    odom_sub = nh.subscribe("odom", 10, &Master:: odometryCallback,this);

    
}

void  Master::odometryCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    pose_v=msg->twist.twist.linear.x;
 
}
void   Master::point_Call_back(const std_msgs::Int32  msg)
{

     sleep(0.2);
     if (msg.data > 3)
     {
         open_point=true;
     }
     else
     {
        open_point=false;
     }
}

void Master::Call_back_sub(const my_msg_pkg::remot_msg   & req)
{
    int  new_state=0;
     std::cout<<"会调相应"<<std::endl;//
      std_msgs::String msg;
      if (req.request == 14)
      {
          bool_raw=false;
          std::cout<<bool_raw<<std::endl;
          return;
      }

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
            if(select_vel==3 || select_vel==4)
            {
                std::cout<<"bool_raw=true;"<<std::endl;
                bool_raw=true;
            }
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
                            int result= system("/bin/bash -c 'source /home/ft/ft_robot_ws/map_sav.sh'");
                             std::cout<<result<<std::endl;
                             sleep(2);
                            if(result==0)
                            {
                                sleep(1);
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
                             Pre_state=STOP_STATE;
                            gmapping_start=false;
                            // int result= system("source  /home/ft/ft_robot_ws/amcl.sh ");
                             int resuleeet= system("/bin/bash -c 'source /home/ft/ft_robot_ws/my_amcl.sh'");
                            //  int resulwwt = system("/home/ft/ft_robot_ws/map_sav.sh");
                            std::cout<<result<<std::endl;
                             sleep(2);
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
                            msg.data = "建图功能未启动.mp3";
                            chatter_pub.publish(msg);
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
                all_rout.clear();
                if(In_file(rout))
                {
                    all_rout=rout;
                    pub_point();
                    //删除多余点
                    get_delete_point_path();
                    pub_pointr();
                    //平滑路径
                    get_path();
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
                open_point=false;
            }
            // biao_dian=false;
            nav_vel=false;
             contr_vel=true;
            select_vel=0;  
            if(gmapping_start==false &&  biao_dian==false)
            {
                 Pre_state=STOP_STATE;
            }
    
        }
        else  if(Req_state==SIGN_STATE)  //标点
        {
            if (Pre_state == SIGN_STATE)
            {
                if (req.request == 9)
                {
                    open_luzhi = true;
                    msg.data = "开始标记导航点.mp3";
                    chatter_pub.publish(msg);
                }
                else if(req.request == 10)
                {
                    if (Judge_file())
                    {
                        if (clear_file())
                        {
                            std::cout << "删除导航点成功" << std::endl;
                            msg.data = "导航点清除.mp3";
                            chatter_pub.publish(msg);
                        }
                        else
                        {
                            std::cout << "导航点清除失败" << std::endl;
                            msg.data = "导航点清除失败.mp3";
                            chatter_pub.publish(msg);
                        }
                    }
                    else
                    {
                        std::cout << "文件为空" << std::endl;
                    }
                }
                else
                {
                    std::cout<<"重复进入标记状态"<<std::endl;
                    msg.data = "标记导航点功能已退出.mp3";
                    chatter_pub.publish(msg);
                    Pre_state=STOP_STATE;
                    biao_dian=false;
                    open_luzhi=false;
                }
            }
            else
            {
                if(req.request == 8)
                {
                    Pre_state = SIGN_STATE;
                    biao_dian = true;
                    msg.data = "导航点标记功能启动成功.mp3";
                    chatter_pub.publish(msg);
                }
                else
                {
                    std::cout << "未进入标记" << std::endl;
                    msg.data = "导航点标记功能未启动.mp3";
                    chatter_pub.publish(msg);
                }
            }

         }
         //其余
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
        std_msgs::String msg;
        std::cout << "文件没有数据。" << std::endl;
         msg.data = "文件空.mp3";
        chatter_pub.publish(msg);
    } else {
        file.seekg(0, std::ios::beg);
        std::cout << "文件包含数据。" << std::endl;
        file.close();
        return true;
    }
    file.close();
    return  false;
};
// #define  SPD 0.5
void Master:: pub_vel()
{
    while( contr_vel)//停止按钮
    {
       
        // std::cout << "速度控制-----" << std::endl;
        usleep(200000);
        geometry_msgs::Twist twist;
        double  vel=0.0;
        double alpha=0.35;
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
            ver_control=vel;
            cmd_vel_pub.publish(twist);
        }
        else if (select_vel == 2) // 后退
        {
            // std::cout << "后退-----" << std::endl;
            twist.linear.x = -vel; // 线速度，沿x轴，单位：米/秒
            twist.angular.z = 0.0; // 角速度，绕z轴，单位：弧度/秒
            cmd_vel_pub.publish(twist);
            ver_control = -vel;
        }
        else if (select_vel == 3) // 左
        {
            if (bool_raw)
            {
                 std::cout<<"lift_____-------------------"<<bool_raw<<std::endl;
                twist.linear.x = ver_control; // 线速度，沿x轴，单位：米/秒
                twist.angular.z = alpha;      // 角速度，绕z轴，单位：弧度/秒
                cmd_vel_pub.publish(twist);
            }
            else{
                std::cout<<"lift-------------------"<<bool_raw<<std::endl;
                twist.linear.x = ver_control; // 线速度，沿x轴，单位：米/秒
                twist.angular.z = 0.0;      // 角速度，绕z轴，单位：弧度/秒
                cmd_vel_pub.publish(twist);
            }
        }
        else if (select_vel == 4) // 右
        {
             std::cout << "右-----"<<bool_raw << std::endl;
            if (bool_raw)
            {
                std::cout<<"right__------------------"<<bool_raw<<std::endl;
                twist.linear.x = ver_control; // 线速度，沿x轴，单位：米/秒
                twist.angular.z = -alpha;     // 角速度，绕z轴，单位：弧度/秒
                cmd_vel_pub.publish(twist);
            }
            else
            {
                std::cout << "right__-------------------" <<bool_raw<< std::endl;
                twist.linear.x = ver_control; // 线速度，沿x轴，单位：米/秒
                twist.angular.z = 0.0;        // 角速度，绕z轴，单位：弧度/秒
                cmd_vel_pub.publish(twist);
            }
        }
           else  if(select_vel==11) //左后
        {
            //  std::cout << "左后-----" << std::endl;
             twist.linear.x = -vel;  // 线速度，沿x轴，单位：米/秒
            twist.angular.z = alpha;// 角速度，绕z轴，单位：弧度/秒
            cmd_vel_pub.publish(twist);
            ver_control=-vel;
        }
         else  if(select_vel==12) //右后
        {
             twist.linear.x = -vel;  // 线速度，沿x轴，单位：米/秒
            twist.angular.z = -alpha;// 角速度，绕z轴，单位：弧度/秒
            cmd_vel_pub.publish(twist);
            ver_control=-vel;
        }
        else if(select_vel==0) //停止
        {  
          twist.linear.x = 0.0;  // 线速度，沿x轴，单位：米/秒
          twist.angular.z = 0.0 ;// 角速度，绕z轴，单位：弧度/秒
          cmd_vel_pub.publish(twist);
          ver_control=0.0;
        }
        else if(select_vel==7) //停止
        {   
       
        }
        else
        {
             twist.linear.x = 0.0;
             twist.angular.z = 0.0 ;
             cmd_vel_pub.publish(twist);
             ver_control=0.0;

        }
        
    }

};
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

    pose_raw = yaw;
    if (open_luzhi == true)
    {
        double cost = std::sqrt(std::pow(data[0] - pose_x, 2) + std::pow(data[1] - pose_y, 2));

    if (open_luzhi == true)
        std::cout << "open_luzhi:" << open_luzhi << std::endl;
        if (cost > 1.0 &&   pose_v > 0.0)
        {
            data[0] = pose_x;
            data[1] = pose_y;
            data[2] = pose_raw;
            std::cout << "data:" << data[0] << "------------" << data[1] << " -------" << data[2] << std::endl;
            std_msgs::String msg;
            if (Out_file(data))
            {
                std::cout << "导航点标记成功" << std::endl;
                msg.data = "导航点标记成功.mp3";
                chatter_pub.publish(msg);
            }
            else
            {
                std::cout << "导航点标记失败" << std::endl;
                msg.data = "导航点标记失败.mp3";
                chatter_pub.publish(msg);
            }
        }
    }
}

void Master::nav_worker_thread()
 {
    while (open_start)
    {
        std::cout << "nav:" << nav_vel << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        if (nav_vel == true)
        {
            int indx = -1; // 预瞄点
            int path_size = this->all_rout.size() - 1;
            std::cout << "路径点个数：" << path_size << std::endl;
            double delta = 0;
            double car_cost = 1000;
            int size_data = 0;
            //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // 预瞄点与最近点
            geometry_msgs::PointStamped this_point_stamped;
            std_msgs::String msg;
            while (true)
            {
                double targetSpeed = 0.3; // 期望速度
                double Kv = 0.05;         // 预瞄系数
                double Kp = 1;            // 加速度系数
                double Ld = 1;            // 预测距离
                double L = 0.58;          // 车轴距
                double dt = 0.1;          // 轮距
                double a = 0.3;
                // 获得预眇点
                // 1.路径最近点
                int min_idx = -1;
                double min_cost = 100000;
                geometry_msgs::Twist twist;
                for (int i = 0; i < path_size; i++)
                {
                    double cost = std::sqrt(std::pow(all_rout[i][0] - pose_x, 2) + std::pow(all_rout[i][1] - pose_y, 2));
                    if (cost <= min_cost)
                    {
                        min_cost = cost;
                        min_idx = i;
                    }
                }
                this_point_stamped.header.stamp = ros::Time::now();
                this_point_stamped.header.frame_id = "map";
                this_point_stamped.point.x = all_rout[min_idx][0];
                this_point_stamped.point.y = all_rout[min_idx][1];
                this_point_stamped.point.z = 0.0;
                min_point_pub.publish(this_point_stamped);
                // 2.预眇点
                Ld = pose_v * Kv + Ld;
                if ( (path_size -min_idx<6))
                {
                    min_idx= 1;
                }
                else
                {
                    double L_cost = 0;
                    int pre_indx = min_idx;
                    for (int i = min_idx + 1; i < path_size  &&  i<min_idx+20; i++)
                    {
                        L_cost = L_cost + std::sqrt(std::pow(all_rout[i][0] - all_rout[i - 1][0], 2) + std::pow(all_rout[i][1] - all_rout[i - 1][1], 2));
                        if (L_cost > Ld)
                        {
                            pre_indx = i - 1;
    
                            break;
                        }
                        pre_indx=i;
                    }
                    indx = pre_indx;
                }


                this_point_stamped.header.stamp = ros::Time::now();
                this_point_stamped.header.frame_id = "map";
                this_point_stamped.point.x = all_rout[indx][0];
                this_point_stamped.point.y = all_rout[indx][1];
                this_point_stamped.point.z = 0.0;
                pre_point_pub.publish(this_point_stamped);
                // 计算控制量
                double d_x = this->all_rout[indx][0] - pose_x;
                double d_y = this->all_rout[indx][1] - pose_y;
                double alpha = std::atan2(d_y, d_x) - this->pose_raw;
                double v = 0.2;
                // 计算控制量
                if (path_size - min_idx < 8)
                {
                    std::cout << "size_data" << size_data << std::endl;
                    msg.data = "巡检机器人已到达终点.mp3";
                    chatter_pub.publish(msg);
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                    size_data++;
                }
                if (alpha > 3.14)
                {
                    alpha = alpha - 6.28;
                }
                if (alpha < -3.14)
                {
                    alpha = alpha + 6.28;
                }

                twist.linear.x = v;      // 线速度，沿x轴，单位：米/秒
                twist.angular.z = alpha; // 角速度，绕z轴，单位：弧度/秒
                std::cout << "dy/dx:" << std::atan2(d_y, d_x) << std::endl;
                std::cout << "预瞄点：" << all_rout[indx][2] << std::endl;
                std::cout << "车辆：" << pose_raw << std::endl;
                std::cout << "控制量：" << alpha << std::endl;
                std::cout << "控制量v:" << v << std::endl;

                cmd_vel_pub.publish(twist);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                std::cout << "巡检进行中" << std::endl;
                if (open_point)
                {
                    geometry_msgs::Twist twist;
                    twist.linear.x = 0.0;
                    twist.angular.z = 0.0;
                    int l_size = 0;
                    while (open_point)
                    {
                        std::cout << "障碍物阻塞" << std::endl;
                        cmd_vel_pub.publish(twist);
                        sleep(1);
                        if (l_size % 200 == 0)
                        {
                            msg.data = "检测到障碍物车辆已暂停.mp3";
                            chatter_pub.publish(msg);
                            std::cout << "l_size:" << l_size << std::endl;
                        }
                        l_size++;
                        std::cout << "障碍以清除" << std::endl;
                        if (!nav_vel)
                        {
                            break;
                        }
                    }
                }
                if (!nav_vel)
                {
                    break;
                }
            };
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
 };

int   Master::Get_Refpoint_indx( int &  L_indx)
{
    
}

void  Master:: pub_point()
{
    nav_msgs::Path path;
    std::cout << "路径开始发布" << std::endl;
    for (auto it = all_rout.begin(); it != all_rout.end(); it++)
    {

        ros::Time current_time, last_time;
        current_time = ros::Time::now(); // 记录当前时间
        // 设置路径消息的时间戳和坐标系
        path.header.stamp = current_time;
        path.header.frame_id = "map";
        geometry_msgs::PoseStamped this_pose_stamped;
        this_pose_stamped.pose.position.x = (*it)[0];
        this_pose_stamped.pose.position.y = (*it)[1];
        this_pose_stamped.pose.position.z = 0.0;
        this_pose_stamped.header.stamp = current_time;
        this_pose_stamped.header.frame_id = "map";
        tf2::Quaternion qtn;
        qtn.setRPY(0, 0, (*it)[2]);
        this_pose_stamped.pose.orientation.x = qtn.getX();
        this_pose_stamped.pose.orientation.y = qtn.getY();
        this_pose_stamped.pose.orientation.z = qtn.getZ();
        this_pose_stamped.pose.orientation.w = qtn.getW(); // 四元数
        // 设置姿态消息的时间戳和坐标系
        std::cout << "路径以发布" << std::endl;
        // 将新的姿态添加到路径中
        path.poses.push_back(this_pose_stamped);
    }
    path_pub.publish(path);
    std::cout << "路径以发布" << std::endl;
}

void  Master:: pub_pointr()
{
    nav_msgs::Path path;
    std::cout << "路径开始发布" << std::endl;
    for (auto it = all_rout.begin(); it != all_rout.end(); it++)
    {

        ros::Time current_time, last_time;
        current_time = ros::Time::now(); // 记录当前时间
        // 设置路径消息的时间戳和坐标系
        path.header.stamp = current_time;
        path.header.frame_id = "map";
        geometry_msgs::PoseStamped this_pose_stamped;
        this_pose_stamped.pose.position.x = (*it)[0];
        this_pose_stamped.pose.position.y = (*it)[1];
        this_pose_stamped.pose.position.z = 0.0;
        this_pose_stamped.header.stamp = current_time;
        this_pose_stamped.header.frame_id = "map";
        tf2::Quaternion qtn;
        qtn.setRPY(0, 0, (*it)[2]);
        this_pose_stamped.pose.orientation.x = qtn.getX();
        this_pose_stamped.pose.orientation.y = qtn.getY();
        this_pose_stamped.pose.orientation.z = qtn.getZ();
        this_pose_stamped.pose.orientation.w = qtn.getW(); // 四元数
        // 设置姿态消息的时间戳和坐标系
        std::cout << "路径以发布" << std::endl;
        // 将新的姿态添加到路径中
        path.poses.push_back(this_pose_stamped);
    }
    path_pubr.publish(path);
    std::cout << "路径以发布" << std::endl;
}

double Master:: BaseFunction(int i,double u,int k,const std::vector<double> &knof)
{
    double Bik_u;
    if (k == 1) // 0次🅱样条，阶数1
    {
        if (u >= knof[i] && u < knof[i + 1])
        {
            Bik_u = 1;
        }
        else
        {
            Bik_u = 0;
        }
    }
    else
    {
        // double Length1 = knof[i + k - 1] - knof[i];
        // double Length2 = knof[i + k] - knof[i + 1];
        double Length1 = knof[i + k - 1] - knof[i];
        double Length2 = knof[i + k] - knof[i + 1];
        //避免遇见0/0，当分母同为0时约定分母为1，此时分子为0整体为0
        if(Length1==0)
        Length1=1;
        if(Length2==0)
        Length2=1;
        Bik_u = (u - knof[i]) / Length1 * BaseFunction(i,u,k-1,knof) + (knof[i + k] - u) / Length2 * BaseFunction(i+1,u,k-1,knof);
    }
    return Bik_u;
}

void  Master::get_konf (int n,int k,std::vector<double>  &knof)
{
    double  duanshu=n-k; 
    std::cout<<duanshu<<"值"<<(1/duanshu)<<std::endl;
    for(int i=0;i<=k;i++)   
    knof[i]=0;
    if(duanshu>0)
    {
        for(int i=k+1;i<n+1;i++)
        knof[i]=knof[i-1]+1/(duanshu+1);      
    }
    for(int i=n+1;i<=n+k+1;i++)
    knof[i]=1;
}

void  Master::get_path()
{
    int n=all_rout.size()-1;
    int k=4;
    std::vector<double> knof(n + k + 1);
    get_konf(n, k - 1, knof);
    for (double v : knof)
        std::cout << v << std::endl;
    // 定义基函数
    Eigen::MatrixXd Bik_fuc(n + 1, 1);
    Eigen::MatrixXd P(2, n+1);
    std::vector<double> path_x;
    std::vector<double> path_y;
    for (int i = 0; i <= n; i++)
    {
        P(0, i) = all_rout[i][0];
        P(1, i) = all_rout[i][1];
    }
    std::cout << "Matrix P:\n"
              << P << std::endl;

    for (double u = 0.0; u < 1; u += 0.005)
    {
        for (int i = 0; i <= n; i++)
        {
            Bik_fuc(i, 0) = BaseFunction(i, u, k, knof);
        }
        Eigen::MatrixXd result = P * Bik_fuc;
        path_x.push_back(result(0, 0));
        path_y.push_back(result(1, 0));
    }
    std::cout << "Matrix P:\n"
              << Bik_fuc << std::endl;

std::cout<<"1数量"<<path_y.size()<<std::endl;
    
    // 求解路径点曲率
    std::vector<double> p_x;
    std::vector<double> p_y;
    std::vector<double> heading;
    for (int i = 0; i < path_x.size() - 1; i++)
    {
        p_x.push_back(path_x[i + 1] - path_x[i]);
        p_y.push_back(path_y[i + 1] - path_y[i]);
    }

    p_x.push_back(p_x[p_x.size() - 1]);
    p_y.push_back(p_y[p_y.size() - 1]);
    for (int i = 0; i < p_x.size(); i++)
    {
        heading.push_back(std::atan2(p_y[i], p_x[i])); // atan2(y, x) 返回一个介于 -π 到 π 之间的值，表示从点 (0, 0) 到点 (x, y) 的向量与正x轴之间的夹角的弧度值
    }
    all_rout.clear();
     for(int i=0;i<path_x.size();i++)
    {
        std::vector<double>ref;
        ref.push_back(path_x[i]);
        ref.push_back(path_y[i]);
        ref.push_back(heading[i]);
        all_rout.push_back(ref);
    }
    std::cout<<"数量"<<all_rout.size()<<std::endl;
    for (auto it = all_rout.begin(); it != all_rout.end(); it++)
    {
        std::cout<<"数量"<<(*it)[0]<<std::endl;
    }
    pub_path();
}

void  Master:: pub_path()
{
    nav_msgs::Path path;
    std::cout << "路径开始发布" << std::endl;
    for (auto it = all_rout.begin(); it != all_rout.end(); it++)
    {
        ros::Time current_time, last_time;
        current_time = ros::Time::now(); // 记录当前时间
        // 设置路径消息的时间戳和坐标系
        path.header.stamp = current_time;
        path.header.frame_id = "map";
        geometry_msgs::PoseStamped this_pose_stamped;
        this_pose_stamped.pose.position.x = (*it)[0];
        this_pose_stamped.pose.position.y = (*it)[1];
        this_pose_stamped.pose.position.z = 0.0;
        this_pose_stamped.header.stamp = current_time;
        this_pose_stamped.header.frame_id = "map";
        tf2::Quaternion qtn;
        qtn.setRPY(0, 0, (*it)[2]);
        this_pose_stamped.pose.orientation.x = qtn.getX();
        this_pose_stamped.pose.orientation.y = qtn.getY();
        this_pose_stamped.pose.orientation.z = qtn.getZ();
        this_pose_stamped.pose.orientation.w = qtn.getW(); // 四元数
        // 设置姿态消息的时间戳和坐标系
        std::cout << "路径以发布" << std::endl;
        // 将新的姿态添加到路径中
        path.poses.push_back(this_pose_stamped);
    }
    B_path_pub.publish(path);
    std::cout << "路径以发布" << std::endl;
}

void Master::get_delete_point_path()
{
    auto it = all_rout.begin();
    while (it != all_rout.end())
    {
        auto temp = std::next(it);
        if (temp == all_rout.end())
            break;
        while (temp != all_rout.end())
        {
            double cost = std::sqrt(
                std::pow((*temp)[0] - (*it)[0], 2) +
                std::pow((*temp)[1] - (*it)[1], 2));
            if (cost < 0.8)
            {
                all_rout.erase(temp);
            }
            else
            {
                ++temp;
            }
        }
        ++it;
    }
    all_rout.push_back(all_rout[0]);
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