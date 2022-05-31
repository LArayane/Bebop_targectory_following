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
    #include <tf/transform_broadcaster.h>
    
  
     double t=0;
     tf::Quaternion Rot;

    int main(int argc, char** argv){

    ros::init(argc, argv, "my_tf_broadcaster"); 
    ros::NodeHandle node;
    ros::Rate rate(100); 

    tf::StampedTransform transform;
    tf::TransformListener listener;
    tf::TransformBroadcaster br;
    tf::Transform transform_;
   
     
     while (node.ok()){

     try{
	   
     if (t==0){
          
     listener.waitForTransform("/odom", "/base_link",ros::Time(0), ros::Duration(3.0));
     listener.lookupTransform("/odom", "/base_link", ros::Time(0), transform);
      Rot = transform.getRotation();
     
     std::cout << "transform exist\n";       
              
         t=1;

       }

     transform_.setOrigin( tf::Vector3 (0.0, 0.0, 0.0) );
     transform_.setRotation(Rot);
     br.sendTransform(tf::StampedTransform(transform_, ros::Time::now(), "odom", "newFrame"));

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
