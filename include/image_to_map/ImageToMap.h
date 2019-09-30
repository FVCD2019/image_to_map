#include <ros/ros.h>
#include <opencv2/highgui.hpp>
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

#ifndef IMAGETOMAP_H
#define IMAGETOMAP_H

class ImageToMap
{
	public:
		ImageToMap();

		void imageCB(const sensor_msgs::ImageConstPtr& image);

	protected:
		
		void parseRawimg(const sensor_msgs::ImageConstPtr& ros_img, cv::Mat& cv_img);
		void MakeMap();
		
		ros::NodeHandle nh;
		ros::Publisher map_pub_;
		ros::Subscriber image_sub_;

		ImageFilter imagefilter;
		cv::Mat frame;
		cv::Mat filter_img;

		std_msgs::Header header;
		nav_msgs::OccupancyGrid i_map;		
		int upper_h, upper_s, upper_v;
		int lower_h, lower_s, lower_v;
		

};

#endif
