#include "ros/ros.h"
#include "move_base_msgs/MoveBaseAction.h"
#include "actionlib/client/simple_action_client.h"
#include"vector"
#include <Eigen/Core>


void  get_konf(int n,int k,std::vector<double>  &knof)
{
    double  duanshu=n-k; //中间节点个数+1
    std::cout<<duanshu<<"值"<<(1/duanshu)<<std::endl;
    for(int i=0;i<=k;i++)   //前K+1个数
    knof[i]=0;
    if(duanshu>0)
    {
        for(int i=k+1;i<n+1;i++)
        knof[i]=knof[i-1]+1/(duanshu+1);      //求中间节点值
    }
    for(int i=n+1;i<=n+k+1;i++)//后k+1个数
    knof[i]=1;
}

double BaseFunction(int i,double u,int k,const std::vector<double> &knof)
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
        // Bik_u=(u-knof[i])/ (Length1*BaseFunction(i,u,k-1,knof)) +(knof[i+k]-u)/ (Length2*BaseFunction(i+1,u,k-1,knof));//
        Bik_u = (u - knof[i]) / Length1 * BaseFunction(i,u,k-1,knof) + (knof[i + k] - u) / Length2 * BaseFunction(i+1,u,k-1,knof);
    }
    return Bik_u;
}

int main(int argc, char** argv)
{
    int n=5;  //控制点个数-1
    int k=4;  //劫数
    std::vector<double>  knof(n+k+1);
    get_konf(n,k-1,knof);
    for(double  v:knof)
    std::cout<<v<<std::endl;
    // 定义基函数
    Eigen:: MatrixXd Bik_fuc(n+1,1);
    //定义控制点
    std::vector<double> X{0,10,25,25,40,50};
    std::vector<double> Y{-1.75,-1.75,-1.25,1.25,1.75,1.75};
    Eigen:: MatrixXd P(2,X.size());
    std::vector<double> path_x;
    std::vector<double>path_y;

    for(int i=0;i<X.size();i++)
    {
        P(0,i)=X[i];
        P(1,i)=Y[i];
    }
     std::cout << "Matrix P:\n" << P <<std:: endl;

    for(double u=0.0;u<1.+0.005;u+=0.005)
    {
        std::cout<<u<<std::endl;
        for (int i=0;i<=n;i++)
        {
            Bik_fuc(i,0)=BaseFunction(i,u,k,knof);
        }
        Eigen::MatrixXd result = P * Bik_fuc;
        path_x.push_back(result(0, 0));
        path_y.push_back(result(1, 0));
    }
    std::cout << "Matrix P:\n"
              << Bik_fuc << std::endl;

    // 求解路径点曲率
    std::vector<std::vector<double>> Ref_path; // 参考路径,三行N列，X,Y,Raw
    Ref_path.push_back(path_x);
    Ref_path.push_back(path_y);
    std::vector<double> p_x;
    std::vector<double> p_y;
    std::vector<double> heading;
    for(int i=0;i<path_x.size()-1;i++)
    {
        p_x.push_back(path_x[i + 1] - path_x[i]);
        p_y.push_back(path_y[i + 1] - path_y[i]);
    }
   
    p_x.push_back(p_x[p_x.size() - 1]);
    p_y.push_back(p_y[p_y.size() - 1]);
    for (int i = 0; i < p_x.size(); i++)
    {
        heading.push_back(std::atan2(p_y[i], p_x[i]));//atan2(y, x) 返回一个介于 -π 到 π 之间的值，表示从点 (0, 0) 到点 (x, y) 的向量与正x轴之间的夹角的弧度值
    }
    Ref_path.push_back(heading);
    for(int i=0;i<Ref_path[2].size();i++)
    {
        std::cout<<Ref_path[0][i]<<"      "<<Ref_path[1][i]<<"      "<<Ref_path[2][i]<<"      "<<std::endl;
    }


    return 0;
}


