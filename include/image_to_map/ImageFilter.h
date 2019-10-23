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
		void backfilter(cv::Mat &filtered_img, int space_id);

	protected:
			
		cv::Scalar upper_back_hsv;
		cv::Scalar lower_back_hsv;
		cv::Scalar upper_back_rgb;
		cv::Scalar lower_back_rgb;
		int upper_h, upper_s, upper_v;
		int lower_h, lower_s, lower_v;
};

#endif
