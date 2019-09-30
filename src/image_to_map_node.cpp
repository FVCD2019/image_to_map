#include "image_to_map/ImageToMap.h"

using namespace std;
using namespace cv;

ImageToMap::ImageToMap()
{
	nh = ros::NodeHandle("~");
	
	map_pub_ = nh.advertise<nav_msgs::OccupancyGrid>("image_map", 1);
	image_sub_ = nh.subscribe("/svm_image", 1, &ImageToMap::imageCB, this);
	
	nh.getParam("upper_h_",upper_h);
	nh.getParam("upper_s_",upper_s);
	nh.getParam("upper_v_",upper_v);
	nh.getParam("lower_h_",lower_h);
	nh.getParam("lower_s_",lower_s);
	nh.getParam("lower_v_",lower_v);

}

void ImageToMap::imageCB(const sensor_msgs::ImageConstPtr& image)
{
	try{
		parseRawimg(image, frame);
	} catch(const cv_bridge::Exception& e) {
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return ;
	} catch(const std::runtime_error& e) {
		cerr << e.what() << endl;
	}
	
	imagefilter.backfilter(frame, filter_img, upper_h, upper_s, upper_v, lower_h, lower_s, lower_v);

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
	int i_width = filter_img.rows;
	int i_height = filter_img.cols;
	int size = i_width * i_height; 
	int8_t i_data[size] = {0};
	int n = 0;	

	for(int i = i_height; i > 0; i--){
		for(int j = 0; j < i_width; j++){
			if(filter_img.at<uchar>(j, i) != 0)
				i_data[n] = 100;
			else
				i_data[n] = 0;
			n++;
		}
	}
	vector<int8_t> i_data_v;
	i_data_v.assign(i_data, i_data+n);

//map 
	header.seq = 0;
	header.frame_id = "/image_map";
	header.stamp = ros::Time::now();
	
	i_map.header = header;

	i_map.info.resolution = 1.0;
	i_map.info.width = i_width;
	i_map.info.height = i_height;
	i_map.info.origin.position.x = 0;
	i_map.info.origin.position.y = 0;
	i_map.info.origin.position.z = 0;
	i_map.info.origin.orientation.x = 0;
	i_map.info.origin.orientation.y = 0;
	i_map.info.origin.orientation.z = 0;
	i_map.info.origin.orientation.w = 1;

	i_map.data = i_data_v;

	map_pub_.publish(i_map);
}
