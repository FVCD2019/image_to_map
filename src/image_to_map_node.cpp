#include "image_to_map/ImageToMap.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{

	ros::init(argc, argv, "image_to_map");
	ImageToMap image_to_map;
	ros::Rate r(30);
	while (ros::ok()){
		image_to_map.run();
		ros::spinOnce();
		r.sleep();
	}

	return 0;
}


ImageToMap::ImageToMap()
{
	nh = ros::NodeHandle();
	
	map_pub_ = nh.advertise<nav_msgs::OccupancyGrid>("map", 1);
	map_pub_local = nh.advertise<nav_msgs::OccupancyGrid>("imap", 1);
	space_sub_ = nh.subscribe("/p_space_id", 1, &ImageToMap::spaceCB, this);
	flag = false;
	count = 0;
	
}

void ImageToMap::spaceCB(const std_msgs::Int16::ConstPtr& msg)
{
	space_id = (msg->data);
	cout << space_id << endl;
	if(count > 20){
		cout << space_id << endl;
		flag =true;
	}else{
		count++;
	}
}

void ImageToMap::run()
{
	MakeIMageMap(space_id,imap);
	imap.copyTo(imap_);
	MakeMap(imap_);
}

void ImageToMap::MakeIMageMap(int space_id, cv::Mat &imap)
{
	if(!flag){
	Mat raw_img(1200,1300,CV_8UC1,Scalar(255));

	Mat img_space_raw;
	Mat img_space;
	raw_img.copyTo(img_space_raw);

	for(int r = 0; r < 1300; r++){
		for(int c = 0; c < 10; c++){
			img_space_raw.at<uchar>(c,r) = 0;
		}
		for(int c = 1190; c < 1200; c++){
			img_space_raw.at<uchar>(c,r) = 0;
		}
	}

	for(int r = 0; r < 10; r++){
		for(int c = 0; c < 1200; c++){
			img_space_raw.at<uchar>(c,r) = 0;
		}
	}

	for(int r = 1290; r < 1300; r++){
		for(int c = 0; c < 1200; c++){
			img_space_raw.at<uchar>(c,r) = 0;
		}
	}


	for(int r = p_list_x[0]-2; r <= p_list_x[5]+2; r++){
		for(int c = p_list_y[0]; c <= p_list_y[1]; c++){
			img_space_raw.at<uchar>(c,r) = 0;
		}
	}

	for(int r = p_list_x[6]; r <= p_list_x[9]; r++){
		for(int c = p_list_y[2]-2; c < p_list_y[3]; c++){
			img_space_raw.at<uchar>(c,r) = 0;
		}
	}
	//cv::imshow("img_space_raw", img_space_raw);
	img_space_raw.copyTo(img_space);
	//cv::imwrite("g_map.png",img_space);
	//cout << "DONE" << endl;
	if(space_id >=5 && space_id < 8){
		space_id = space_id + 1;
	}
	//cout << space_id << endl;

	if (space_id >=0 && space_id < 9){
		for(int r = p_list_x[space_id]-5; r <= p_list_x[space_id+1]+5; r++){
			if (space_id < 5){
				for(int c = p_list_y[0]+10; c <= p_list_y[1]; c++){
					img_space.at<uchar>(c,r) = 255;
				}
			}else{
				for(int c = p_list_y[2]-2; c <= p_list_y[3]-10; c++){
					img_space.at<uchar>(c,r) = 255;
				}
			}
		}
	}

	img_space.copyTo(imap);
	}
	//imshow("img_space", imap);
	//waitKey(3);


}

void ImageToMap::MakeMap(cv::Mat &imap_)
{
	if(!flag){
	imap_.copyTo(convert_img);
	resize(imap_,convert_img,cv::Size(imap_.cols*0.5, imap_.rows*0.5),0.0,CV_INTER_NN);
	//imshow("convert_img", convert_img);
	//waitKey(3);
	int i_width = convert_img.cols;
	int i_height = convert_img.rows;
	//std::cout << i_width << " " << i_height << std::endl;
    	vector<int8_t> i_data_v;

    	for(int j=1; j<=i_height; j++)
    	{
      		for(int i=1; i<=i_width; i++)
      		{
//ROS_INFO("i:%d  j:%d",i,imap_.cols-j);
			if(convert_img.at<uchar>(i_height-j,i) == 0){
			//if(convert_img.at<uchar>(j,i) == 0){
				i_data_v.push_back(100);}
			else{
				i_data_v.push_back(0);}
      		}
    	}
	//std::cout << "size" << i_data_v.size() << std::endl;

	header.seq = 0;
	header.frame_id = "map";
	header.stamp = ros::Time::now();
	
	i_map.header = header;

	i_map.info.resolution = 0.01;
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


//local
	imap_.copyTo(imap_local);
	resize(imap_,imap_local,cv::Size(imap_.cols*0.1, imap_.rows*0.1),0.0);
	//imshow("convert_img", imap_local);
	//waitKey(3);
	int i_width_local = imap_local.cols;
	int i_height_local = imap_local.rows;
	//std::cout << i_width << " " << i_height << std::endl;
    	vector<int8_t> i_data_v_local;

    	for(int j=1; j<=i_height_local; j++)
    	{
      		for(int i=1; i<=i_width_local; i++)
      		{
//ROS_INFO("i:%d  j:%d",i,imap_.cols-j);
			//if(imap_local.at<uchar>(i_height_local-j,i) == 0){
			if(imap_local.at<uchar>(i_height_local-j,i) == 0){
				i_data_v_local.push_back(100);}
			else{
				i_data_v_local.push_back(0);}
      		}
    	}
	//std::cout << "size" << i_data_v.size() << std::endl;

	header.seq = 0;
	header.frame_id = "map";
	header.stamp = ros::Time::now();
	
	i_map_local.header = header;

	i_map_local.info.resolution = 0.05;
	i_map_local.info.width = i_width_local;
	i_map_local.info.height = i_height_local;
	i_map_local.info.origin.position.x = 0;
	i_map_local.info.origin.position.y = 0;
	i_map_local.info.origin.position.z = 0;
	i_map_local.info.origin.orientation.x = 0;
	i_map_local.info.origin.orientation.y = 0;
	i_map_local.info.origin.orientation.z = 0;
	i_map_local.info.origin.orientation.w = 1;

	i_map_local.data = i_data_v_local;

	map_pub_local.publish(i_map_local);
	}else{
		map_pub_.publish(i_map);
		map_pub_local.publish(i_map_local);
	}

}
