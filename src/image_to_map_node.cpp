#include "image_to_map/ImageToMap.h"

using namespace std;
using namespace cv;

ImageToMap::ImageToMap()
{
	nh = ros::NodeHandle("~");
	
	map_pub_ = nh.advertise<nav_msgs::OccupancyGrid>("image_map", 1);
	space_sub_ = nh.subscribe("/detector/p_space", 1, &ImageToMap::spaceCB, this);
	

}

void ImageToMap::spaceCB(const std_msgs::Int16::ConstPtr& msg)
{

	space_id = (msg->data) -1;
	MakeIMageMap(space_id,imap_);
	MakeMap(imap_);

}


void ImageToMap::MakeIMageMap(int space_id, cv::Mat imap_)
{
	Mat raw_img(1150,1200,CV_8UC1,Scalar(255));

	Mat img_space_raw;
	Mat img_space;
	raw_img.copyTo(img_space_raw);

	for(int r = 100; r <= 1000; r++){
		for(int c = 20; c <= 330; c++){
			img_space_raw.at<uchar>(c,r) = 0;
		}
	}

	for(int r = 110; r <= 650; r++){
		for(int c = 810; c <= 1120; c++){
			img_space_raw.at<uchar>(c,r) = 0;
		}
	}

	img_space_raw.copyTo(img_space);

	for(int r = p_list_lt_x[space_id]; r <= p_list_rt_x[space_id]; r++){
		for(int c = p_list_lt_y[space_id]; c <= p_list_rt_y[space_id]; c++){
			img_space.at<uchar>(c,r) = 255;
		}
	}

	img_space.copyTo(imap_);
	imshow("img_space", img_space);

	waitKey(3);


}

void ImageToMap::MakeMap(cv::Mat imap)
{
	int i_width = imap.rows;
	int i_height = imap.cols;
	int size = i_width * i_height; 
	int8_t i_data[size] = {0};
	int n = 0;	

	for(int i = i_height; i > 0; i--){
		for(int j = 0; j < i_width; j++){
			if(imap.at<uchar>(j, i) != 0)
				i_data[n] = 0;
			else
				i_data[n] = 100;
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
