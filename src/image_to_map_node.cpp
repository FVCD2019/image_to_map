#include "image_to_map/ImageToMap.h"

using namespace std;
using namespace cv;

ImageToMap::ImageToMap()
{
	nh = ros::NodeHandle();
	
	map_pub_ = nh.advertise<nav_msgs::OccupancyGrid>("imap", 1);
	space_sub_ = nh.subscribe("/detector/p_space", 1, &ImageToMap::spaceCB, this);
	
}

void ImageToMap::spaceCB(const std_msgs::Int16::ConstPtr& msg)
{
	space_id = (msg->data) -1;
	MakeIMageMap(space_id,imap);
	imap.copyTo(imap_);
	MakeMap(imap_);
}


void ImageToMap::MakeIMageMap(int space_id, cv::Mat &imap)
{
	Mat raw_img(1200,1500,CV_8UC1,Scalar(255));

	Mat img_space_raw;
	Mat img_space;
	raw_img.copyTo(img_space_raw);

	for(int r = 0; r < 1500; r++){
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

	for(int r = 1490; r < 1500; r++){
		for(int c = 0; c < 1200; c++){
			img_space_raw.at<uchar>(c,r) = 0;
		}
	}


	for(int r = p_list_x[0]-2; r <= p_list_x[5]+2; r++){
		for(int c = p_list_y[0]; c <= p_list_y[1]+2; c++){
			img_space_raw.at<uchar>(c,r) = 0;
		}
	}

	for(int r = p_list_x[6]; r <= p_list_x[9]; r++){
		for(int c = p_list_y[2]-2; c <= p_list_y[3]; c++){
			img_space_raw.at<uchar>(c,r) = 0;
		}
	}

//
/*
	for(int r = p_list_lt_x[0]+2; r <= p_list_rt_x[0]-1; r++){
		for(int c = p_list_lt_y[0]+2; c <= p_list_lb_y[0]; c++){
			img_space_raw.at<uchar>(c,r) = 255;
		}
	}


	for(int r = p_list_lt_x[1]+1; r <= p_list_rt_x[1]-1; r++){
		for(int c = p_list_lt_y[0]+2; c <= p_list_lb_y[0]; c++){
			img_space_raw.at<uchar>(c,r) = 255;
		}
	}

	for(int r = p_list_lt_x[2]+1; r <= p_list_rt_x[2]-1; r++){
		for(int c = p_list_lt_y[0]+2; c <= p_list_lb_y[0]; c++){
			img_space_raw.at<uchar>(c,r) = 255;
		}
	}

	for(int r = p_list_lt_x[3]+1; r <= p_list_rt_x[3]-1; r++){
		for(int c = p_list_lt_y[0]+2; c <= p_list_lb_y[0]; c++){
			img_space_raw.at<uchar>(c,r) = 255;
		}
	}

	for(int r = p_list_lt_x[4]+1; r <= p_list_rt_x[4]-2; r++){
		for(int c = p_list_lt_y[0]+2; c <= p_list_lb_y[0]; c++){
			img_space_raw.at<uchar>(c,r) = 255;
		}
	}

	for(int r = p_list_lt_x[5]+2; r <= p_list_rt_x[5]-1; r++){
		for(int c = p_list_lt_y[5]; c <= p_list_lb_y[5]-2; c++){
			img_space_raw.at<uchar>(c,r) = 255;
		}
	}

	for(int r = p_list_lt_x[6]+1; r <= p_list_rt_x[6]-1; r++){
		for(int c = p_list_lt_y[5]; c <= p_list_lb_y[5]-2; c++){
			img_space_raw.at<uchar>(c,r) = 255;
		}
	}

	for(int r = p_list_lt_x[7]+1; r <= p_list_rt_x[7]-2; r++){
		for(int c = p_list_lt_y[5]; c <= p_list_lb_y[5]-2; c++){
			img_space_raw.at<uchar>(c,r) = 255;
		}
	}
*/
//

	//cv::imshow("img_space_raw", img_space_raw);
	img_space_raw.copyTo(img_space);
	//cv::imwrite("g_map.png",img_space);

//ROS_INFO("space_id : %d" , space_id);

/*
ROS_INFO(" lt %d" , p_list_lt_x[space_id]);
ROS_INFO(" rt %d" , p_list_rb_x[space_id]);
ROS_INFO(" lt %d" , p_list_lt_y[space_id]);
ROS_INFO(" rb %d" , p_list_rb_y[space_id]);
*/

	if (space_id >=0 && space_id < 8){
		for(int r = p_list_x[space_id]+2; r <= p_list_x[space_id+1]-2; r++){
			if (space_id < 5){
				for(int c = p_list_y[0]+10; c <= p_list_y[1]+2; c++){
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
	//imshow("img_space", imap);

	//waitKey(3);


}

void ImageToMap::MakeMap(cv::Mat &imap_)
{
	imap_.copyTo(convert_img);
	resize(imap_,convert_img,cv::Size(imap_.cols*0.25, imap_.rows*0.25),0.0,CV_INTER_NN);
	//imshow("convert_img", convert_img);

	//waitKey(3);
	int i_width = convert_img.cols;
	int i_height = convert_img.rows;
	//std::cout << i_width << " " << i_height << std::endl;
    	vector<int8_t> i_data_v;

    	for(int j=1; j<i_height; j++)
    	{
      		for(int i=0; i<i_width; i++)
      		{
//ROS_INFO("i:%d  j:%d",i,imap_.cols-j);
			if(convert_img.at<uchar>(i_height-j,i) == 0){
				i_data_v.push_back(100);}
			else{
				i_data_v.push_back(0);}
      		}
    	}

	header.seq = 0;
	header.frame_id = "map";
	header.stamp = ros::Time::now();
	
	i_map.header = header;

	i_map.info.resolution = 0.02;
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
