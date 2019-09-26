#include <ros/ros.h>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Header>
#include <sensor_msgs/image.h>

#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

class ImageFilter
{
	public:
		cv::Mat backfilter(cv::Mat raw_image, Mat &filtered_img);

	protected:
			
		cv::Mat hsv_img;
		cv::Scalar upper_back_hsv;
		cv::Scalar lower_back_hsv;
		

};

