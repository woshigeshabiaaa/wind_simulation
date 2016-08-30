//
// Created by flyinsky on 16-5-31.
//

#include <ros/ros.h>
//#include <image_transport/image_transport.h>
//#include <opencv2/highgui/highgui.hpp>
//#include <cv_bridge/cv_bridge.h>
//#include <aruco/aruco.h>
#include <iostream>
#include <mavros_msgs/OverrideRCIn.h>
#include <mavros_msgs/State.h>


#define FACTOR  0.6

#define MINRC   1100
#define BASERC  1500
#define MAXRC   1900

// Subscriber to flight mode
ros::Subscriber mavros_state_sub;

// RC publisher
ros::Publisher pub;

// Time control
ros::Time lastTime;

double Roll, Pitch;

// Flight mode
std::string mode;
bool guided;
bool armed;


void mavrosStateCb(const mavros_msgs::StateConstPtr &msg) {
    if (msg->mode == std::string("CMODE(0)"))
        return;
    //ROS_INFO("I heard: [%s] [%d] [%d]", msg->mode.c_str(), msg->armed, msg->guided);
    mode = msg->mode;
    guided = msg->guided == 128;
    armed = msg->armed == 128;
}


int main(int argc, char **argv) {
    ros::init(argc, argv, "vel_pub");
    ros::NodeHandle nh;
    srand(time(0));
    mavros_state_sub = nh.subscribe("/mavros/state", 1, mavrosStateCb);
    mavros_msgs::OverrideRCIn msg;

    while (mode == "LOITER") {


        msg.channels[0] = 0;     //Roll
        msg.channels[1] = 0;    //Pitch
        msg.channels[2] = BASERC+(rand()/RAND_MAX-0.5)*(MAXRC-MINRC);   //Throttle
        msg.channels[3] = 0;        //Yaw
        msg.channels[4] = 0;
        msg.channels[5] = 0;
        msg.channels[6] = 0;
        msg.channels[7] = 0;

        pub.publish(msg);


        pub = nh.advertise<mavros_msgs::OverrideRCIn>("/mavros/rc/override", 10);;
        ros::spin();
    }
}
