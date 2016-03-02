
#include <bits/stdc++.h>
#include "geometry_msgs/Point.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "ros/ros.h"
#include <cv_bridge/cv_bridge.h>



using namespace cv;
using namespace std;



Mat img=imread("/home/surya/catkin_ws/src/beginner_tutorials/src/ps1.png",1);


void chatterCallback(const geometry_msgs::Point :: ConstPtr& m)
{
    geometry_msgs::Point t;
    t.x=m->x;
    t.y=m->y;
    img.at<Vec3b>(t.x,t.y)[0]=255;
    img.at<Vec3b>(t.x,t.y)[1]=0;
    img.at<Vec3b>(t.x,t.y)[2]=0;
    
    
    imshow("Final",img);
    waitKey(200);
    cout<<t.x<<" "<<t.y<<"\n";
}


int main(int argc,char **argv)
{
    namedWindow("Finally",CV_WINDOW_AUTOSIZE);
    
    
   
    ros::init(argc,argv,"listener");
    ros::NodeHandle n;
    ros::Subscriber sub=n.subscribe("chatter",1000,chatterCallback);
    ros::spin();
    
    return 0;
}

    


