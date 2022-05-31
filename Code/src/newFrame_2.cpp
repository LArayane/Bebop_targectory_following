    #include <ros/ros.h>
    #include <geometry_msgs/PointStamped.h>
    #include <geometry_msgs/Vector3.h>
    #include <tf/transform_listener.h>
    #include <geometry_msgs/Twist.h>
    #include <std_msgs/Float32.h>
    #include <cmath>
    #include <cstdlib>
    #include <std_msgs/String.h>
    #include<nav_msgs/Odometry.h>
 
 ros::Publisher NFrame_pub;
 ros::Subscriber NFrame_sub;
 float crt_x, crt_y, crt_z;
 float current_x, current_y, current_z;

void getpos(const nav_msgs::Odometry& msg){

  crt_x = msg.pose.pose.position.x;
  crt_y = msg.pose.pose.position.y;
  crt_z = msg.pose.pose.position.z;


ROS_INFO_STREAM( "Showing position : "
<< "x = " << crt_x
<< "y = " << crt_y
<< "z = " << crt_z);

}
   
    int main(int argc, char** argv){
    ros::init(argc, argv, "my_new_frame"); 
    ros::NodeHandle node;
    tf::StampedTransform transform;
    tf::TransformListener listener;
     
     NFrame_sub = node.subscribe("/bebop/odom",1000, &getpos) ;
     NFrame_pub = node.advertise<geometry_msgs::PointStamped>("/bebop/newFrame_pos", 1000);

     ros::Rate rate(100);  

     while (node.ok()){

     try{
	   
     listener.waitForTransform("/newFrame","/odom", ros::Time(0), ros::Duration(3.0));
     listener.lookupTransform("/newFrame","/odom",  ros::Time(0), transform);
     std::cout << "transform exist\n";

     geometry_msgs::PointStamped odom_point;
     odom_point.header.frame_id = "odom";
   
   //we'll just use the most recent transform available for our simple example
     odom_point.header.stamp = ros::Time();

      odom_point.point.x = crt_x;
      odom_point.point.y = crt_y;
      odom_point.point.z = crt_z;

     geometry_msgs::PointStamped newFrame_point;
     listener.transformPoint("newFrame", odom_point, newFrame_point);
 
     current_x = newFrame_point.point.x;
     current_y = newFrame_point.point.y;
     current_z = newFrame_point.point.z;

     ROS_INFO("odom: (%.2f, %.2f. %.2f) -----> newFrame: (%.2f, %.2f, %.2f) at time %.2f",
           odom_point.point.x, odom_point.point.y, odom_point.point.z,
           newFrame_point.point.x, newFrame_point.point.y, newFrame_point.point.z, newFrame_point.header.stamp.toSec());      
      NFrame_pub.publish(newFrame_point);

       }
      catch (tf::TransformException &ex) {
         ROS_ERROR("%s",ex.what());
         ros::Duration(1.0).sleep();
         continue;
       }
       
  
       rate.sleep();
       ros::spinOnce();
     }
     return 0;
    };


