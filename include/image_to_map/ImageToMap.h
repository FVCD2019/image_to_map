#include <ros/ros.h>
#include <opencv2/highgui.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include "opencv2/opencv.hpp"
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Header.h>
#include <nav_msgs/MapMetaData.h>
#include <nav_msgs/OccupancyGrid.h>
#include "image_to_map/ImageFilter.h"
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <std_msgs/Int16.h>

#ifndef IMAGETOMAP_H
#define IMAGETOMAP_H

class ImageToMap
{
	public:
		ImageToMap();

		void spaceCB(const std_msgs::Int16::ConstPtr& msg);

	protected:
		void MakeIMageMap(int space_id, cv::Mat &imap_);
		void MakeMap(cv::Mat &imap);
		
		ros::NodeHandle nh;
		ros::Publisher map_pub_;
		ros::Publisher map_pub_local;
		ros::Subscriber space_sub_;

		cv::Mat imap;
		cv::Mat imap_;
		cv::Mat imap_local;
		cv::Mat convert_img;

		std_msgs::Header header;
		nav_msgs::OccupancyGrid i_map;
		nav_msgs::OccupancyGrid i_map_local;

		int space_id;

		int p_list_x[10] = {110,294,470,651,830,1009,111,292,472,651};
		int p_list_y[4] = {0,318,816,1200};

		int p_list_lt_x[8] = {110,294,470,651,830,1009,290,470};
		int p_list_lt_y[8] = {20,20,20,20,20,810,810,810};
		int p_list_lb_x[8] = {100,280,460,640,820,110,290,470};
		int p_list_lb_y[8] = {330,330,330,330,330,1120,1120,1120};
		int p_list_rt_x[8] = {280,460,640,820,1000,290,470,650};
		int p_list_rt_y[8] = {20,20,20,20,20,810,810,810};
		int p_list_rb_x[8] = {280,460,640,820,1000,290,470,650};
		int p_list_rb_y[8] = {330,330,330,330,330,1120,1120,1120};

};

#endif
