#include "image_to_map/ImageToMap.h"

using namespace std;
using namespace cv;

ImageToMap::ImageToMap()
{
	nh = ros::NodeHandle("~");
	
	map_pub_ = nh.advertise<nav_msgs/OccupancyGrid>("image_map", 1);
	image_sub_ = nh.subscribe("/svm_image", 1, &ImageToMap::imageCB, this);

}

void ImageToMap::imageCB(const sercor_msgs::ImageConstPtr& image)
{
	try{
		parseRawimg(image, frame);
	} catch(const cv_bridge::Exception& e) {
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return ;
	} catch(const std::runtime_error& e) {
		cerr << e.what() << endl;
	}
	
	imagefilter.backfilter(frame);

}

void ImageToMap::parseRawimg(const sensor_msgs::ImageConstPtr& ros_img, cv::Mat& cv_img)
{
	cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(ros_img, sensor_msgs::image_encodings::BGR8);
	cv_img = cv_ptr->image;
	if (cv_img.empty()) {
		throw std::runtime_error("frame is empty!");
	}
}

void ImageToMap::MakeMap()
{
	


	map_pub_.publish(image_map);
}
