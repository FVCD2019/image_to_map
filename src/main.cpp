#include <ros/ros.h>
#include <iostream>
#include "opencv2/opencv.hpp"

#include "image_to_map/ImageToMap.h"

int main(int argc, char** argv)
{
	ros::init(argc, argv, "image_to_map");
	ImageToMap image_to_map;

	ros::spin();

	return 0;
}
