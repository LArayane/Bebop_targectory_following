# Trajectory Planning

###### -The code is intended to save the captured images and the camera velocity to validate the SfM approach. 
###### -The ZigZag trajectory was chosen to ensure the observability of the estimation scheme. 
###### -The video can be found here :
https://www.youtube.com/watch?v=fYti40h7yp0
###### -Since it is not possible to modify the onboard controller of the Bebop. The lowest level control of the Bebop is velocity control through the cmd_vel topic,
thats why P controller was applied on the position error and sent through cmd_vel topic.
