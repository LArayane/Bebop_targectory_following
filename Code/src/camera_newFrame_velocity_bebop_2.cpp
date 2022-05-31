    #include <ros/ros.h>
    #include <geometry_msgs/PointStamped.h>
    #include <geometry_msgs/Vector3.h>
    #include <tf/transform_listener.h>
    #include <geometry_msgs/Twist.h>
    #include <std_msgs/Float32.h>
    #include <cmath>
    #include <cstdlib>
    #include <std_msgs/String.h>
    #include <nav_msgs/Odometry.h>
    #include <geometry_msgs/TwistStamped.h>
    #include <sensor_msgs/Image.h>
 
ros::Publisher  cam_vel;
ros::Subscriber cam_vel_sub;


float msg_lin_x, msg_lin_y, msg_lin_z, msg_ang_x, msg_ang_y, msg_ang_z;
sensor_msgs::Image rawImg;

void getVelocity(const nav_msgs::Odometry& msgVel){

msg_lin_x =  msgVel.twist.twist.linear.x;
msg_lin_y =  msgVel.twist.twist.linear.y;
msg_lin_z =  msgVel.twist.twist.linear.z;
msg_ang_x =  msgVel.twist.twist.angular.x;
msg_ang_y =  msgVel.twist.twist.angular.y;
msg_ang_z =  msgVel.twist.twist.angular.z;


ROS_INFO_STREAM( "Showing velocity :\n "
<< "xlin = " << msgVel.twist.twist.linear.x << " " 
<< "ylin = " << msgVel.twist.twist.linear.y << " " 
<< "zlin = " << msgVel.twist.twist.linear.z << " "
<< "xang = " << msgVel.twist.twist.angular.x << " " 
<< "yang = " << msgVel.twist.twist.angular.y << " " 
<< "zang = " << msgVel.twist.twist.angular.z);
}

   
    int main(int argc, char** argv){
    ros::init(argc, argv, "camera_newFrame_bebop_tf_listener_2"); 
    ros::NodeHandle node;
    tf::StampedTransform transform;
    tf::TransformListener listener;
     
     cam_vel_sub = node.subscribe("/bebop/odom",1000, &getVelocity) ;
     cam_vel = node.advertise<geometry_msgs::Twist>("/bebop/camera_optical_cmd_vel", 1000);

    
     ros::Rate rate(100);  
     while (node.ok()){

     try{
	   
     listener.waitForTransform("/camera_optical","/odom", ros::Time(0), ros::Duration(3.0));
     listener.lookupTransform("/camera_optical","/odom", ros::Time(0), transform);
     std::cout << "transform exist\n";

     tf::Vector3 twist_vel(msg_lin_x,
                           msg_lin_y,
                           msg_lin_z);
     tf::Vector3 twist_rot(msg_ang_x,
                           msg_ang_y,
                           msg_ang_z);


  tf::Vector3 out_rot = transform.getBasis()*twist_rot;
  tf::Vector3 out_vel = transform.getBasis()*twist_vel + transform.getOrigin().cross(out_rot);

  geometry_msgs::Twist vel_msg;

  vel_msg.linear.x = out_vel.x();
  vel_msg.linear.y = out_vel.y();
  vel_msg.linear.z = out_vel.z();

  vel_msg.angular.x = out_rot.x(); 
  vel_msg.angular.y = out_rot.y(); 
  vel_msg.angular.z = out_rot.z();    

      
  cam_vel.publish(vel_msg);

ROS_INFO_STREAM( "Showing velocity after transformation w.r.t the camera frame :\n "
<< "xlin = " << out_vel.x() << " " 
<< "ylin = " << out_vel.y()  << " " 
<< "zlin = " << out_vel.z()  << " "
<< "xang = " << out_rot.x()  << " " 
<< "yang = " << out_rot.y()  << " " 
<< "zang = " << out_rot.z());

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


