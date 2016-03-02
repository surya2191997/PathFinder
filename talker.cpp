#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include <queue>
#include "ros/ros.h"
#include"geometry_msgs/Point.h"
#include <cv_bridge/cv_bridge.h>
#include <bits/stdc++.h>


using namespace std;
using namespace cv;
int rmini=100000,rmaxi=-100000,rminj=100000,rmaxj=-100000;
int gmini=100000,gmaxi=-100000,gminj=100000,gmaxj=-100000;

 struct points{
   int x;
   int y;};
   typedef  points point;

   

void RedDetectBGR(Mat img, int min_thresh, int max_thresh){ 
                                                    //the result image matrix is initialized
    int i,j,r = img.rows, c = img.cols;

    for(i=0;i<r;i++){                               //loop through all rows
        for(j=0;j<c;j++){                           //loop through all columns

            Vec3b colours = img.at<Vec3b>(i,j);     //extract 3 channel pixel information in a vector
            if( colours[2]>=max_thresh && colours[1]<min_thresh && colours[0]<min_thresh)
                                                    //enforce condition on uchar value of each channel  
             {
                if(i<rmini)                          //find extreme coordinates of the circle
                rmini=i;
                if(i>rmaxi)
                rmaxi=i;
                if(j<rminj)
                rminj=j;
                if(j>rmaxj)
                rmaxj=j;     
             }              
            
                 
        }
    }

    return ;
}


void GreenDetectBGR(Mat img, int min_thresh, int max_thresh){

                                                    //the result image matrix is initialized
    int i,j,r = img.rows, c = img.cols;

    for(i=0;i<r;i++){                               //loop through all rows
        for(j=0;j<c;j++){                           //loop through all columns

            Vec3b colours = img.at<Vec3b>(i,j);     //extract 3 channel pixel information in a vector
            if(colours[1]>=max_thresh && colours[2]<min_thresh && colours[0]<min_thresh)
                                                    //enforce condition on uchar value of each channel  
             {
                if(i<gmini)                          //find extreme coordinates of the circle
                gmini=i;
                if(i>gmaxi)
                gmaxi=i;
                if(j<gminj)
                gminj=j;
                if(j>gmaxj)
                gmaxj=j;     
             }              
            
                 
        }
    }

    return ;
}


int edge(Mat image,int x,int y)
{
if(x==0||x==image.rows-1||y==0||y==image.cols-1)
return 1;
else
return 0;
}

int main(int argc,char **argv){

 
 Mat image = imread("/home/surya/catkin_ws/src/beginner_tutorials/src/ps1.png");
                    
 int minthresh = 50, maxthresh = 150;            

    
    

   int i,j; 
  
  
   point p[200][200],q[200][200];
  
   int color[200][200];

   for(i=0;i<200;i++)
 {
     for(j=0;j<200;j++)
     {
      p[i][j].x=i;
      p[i][j].y=j;
      q[i][j].x=0;
      q[i][j].y=0;
      color[i][j]=0;
      }
 }   

   std::queue<point> _pointQ;


  
 RedDetectBGR(image,minthresh,maxthresh);
 GreenDetectBGR(image,minthresh,maxthresh);
 

int x1,y1,x2,y2,l,k;

x1=(rmini+rmaxi)/2; x2=(gmini+gmaxi)/2; y1=(rminj+rmaxj)/2 ;y2=(gminj+gmaxj)/2;

int num=230,flag=0;


color[x1][y1]=-1;
_pointQ.push(p[x1][y1]);

while(1)
{
point t=_pointQ.front();
_pointQ.pop();

for(l=1;l>=-1;l--)
{
for(k=1;k>=-1;k--)
{
if(color[t.x+l][t.y+k]==0&&!(image.at<Vec3b>(t.x+l,t.y+l)[0]>num&&image.at<Vec3b>(t.x+l,t.y+k)[1]>num&&image.at<Vec3b>(t.x+l,t.y+k)[2]>num)&&!edge(image,t.x+l,t.y+k))
{
color[t.x+l][t.y+k]=-1; q[t.x+l][t.y+k]=t;
_pointQ.push(p[t.x+l][t.y+k]);
if(t.x+l==x2&&t.y+k==y2)
flag=1; break;
}

}
}
if(flag==1)break;
}

point a;
a.x=x2;
a.y=y2;
 


ros::init(argc, argv, "talker");
ros::NodeHandle n;
ros::Publisher chatter_pub = n.advertise<geometry_msgs::Point>("chatter", 1000); 
ros::Rate loop_rate(10);
int count=0;

  while (1)
     {
       geometry_msgs::Point m;
       m.x=a.x;
       m.y=a.y; 

     chatter_pub.publish(m);
     ros::spinOnce();
     loop_rate.sleep();

  
a=q[a.x][a.y];
if(a.x==x1&&a.y==y1)
{
count++;
a.x=x2;
a.y=y2;
if(count==2)break;
}
}
imshow("Original image",image);
waitKey(0);
return 0;
}
