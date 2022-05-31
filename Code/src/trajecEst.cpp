#include <ros/ros.h>
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32.h>
#include <cmath>
#include <cstdlib>
#include <std_msgs/String.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Empty.h>
float current_x, current_y, current_z;
int in = 0;
int out = 0;
double z;
double x;
double ti=0;
int ind = 1;
#define M_PI 3.14159265358979323846 / * pi */

void poseCallBack(const geometry_msgs::PointStamped& msg) {
  current_x = msg.point.x;
  current_y = msg.point.y;
  current_z = msg.point.z;
  in = 1;
std::cerr << "x: " << current_x << "y: " << current_y << "ti"  << ti << std::endl;  
}

int main(int argc, char ** argv) {
  ros::init(argc, argv, "trajectoryGoal");
  ros::NodeHandle nh_;
  ros::Publisher landPublisher=nh_.advertise<std_msgs::Empty>("/bebop/land", 10);
  ros::Subscriber odom_sub = nh_.subscribe("/bebop/newFrame_pos", 10, & poseCallBack);
  ros::Publisher cmd_vel_pub = nh_.advertise<geometry_msgs::Twist>("/bebop/cmd_vel", 10);


  ros::Rate loop_rate(30);
  geometry_msgs::Point goal;
 
  goal.z = 1;
  
  while (ros::ok()) {
  if (in> 0) {
     z = goal.z - current_z;
 
 std::cerr << "x: " << current_x << "y: " << current_y << std::endl;  
 if (z > 0.01 && out == 0) {
        out = 0;
}else {
  out = 1;
ROS_INFO("I reached");
}

if (out == 0) {
        geometry_msgs::Twist cmd;
        cmd.linear.z= 0.4*(goal.z - current_z);
        cmd_vel_pub.publish(cmd);

std::cerr << "goal: " << goal.z << "current: " << current_z << std::endl;  

      } else{ 

if (current_x > -4.5 && ti < 30){
ti = ti + .03;

if (current_y > -1 && ind == 2) {
        
        geometry_msgs::Twist cmd;
        cmd.linear.y =  0.12*(-1 - current_y);
        cmd.linear.z =  0.15*(2 - current_z);
        cmd.linear.x = -.01*ti;
        cmd_vel_pub.publish(cmd);

std::cerr << "x: " << current_x << "y: " << current_y << "ti" << ti <<std::endl; 
//std::cerr << "cmd_vel_x: " << cmd.linear.x << "cmd_vel_y: " << cmd.linear.y << "cmd_vel_z:" << cmd.linear.z <<std::endl;   

}
else
{
ind = 1;
} 

if (current_y < 1 && ind == 1) {
        
        geometry_msgs::Twist cmd;
        cmd.linear.y =  0.12*(1 - current_y);
        cmd.linear.z =  0.15*(1 - current_z);
        cmd.linear.x = -.01*ti;
         cmd_vel_pub.publish(cmd);
std::cerr << "x: " << current_x << "y: " << current_y << "ti" << ti <<std::endl; 
//std::cerr << "cmd_vel_x: " << cmd.linear.x << "cmd_vel_y: " << cmd.linear.y << "cmd_vel_z:" << cmd.linear.z <<std::endl;   

}
else
{
ind = 2;
} 
}

else{ 
ROS_INFO("am landing");
      std_msgs::Empty cmd;
      landPublisher.publish(cmd);    
      ROS_INFO("Out : %d", out );         
        }
}
      loop_rate.sleep();
    }
    ros::spinOnce();
}
  return 0;
}
