#include <ros/ros.h>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Header.h>
#include <sensor_msgs/Image.h>

#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

class ImageFilter
{
	public:
		void backfilter(cv::Mat raw_image, cv::Mat &filtered_img);

	protected:
			
		cv::Scalar upper_back_hsv;
		cv::Scalar lower_back_hsv;	

};

#endif
