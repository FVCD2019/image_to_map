#include <image_to_map/ImageFilter.h>

using namespace std;
using namespace cv;

void ImageFilter::backfilter(Mat raw_image, Mat &filtered_img)
{

	nh.getParam("upper_h_",upper_h);
	nh.getParam("upper_s_",upper_s);
	nh.getParam("upper_v_",upper_v);
	nh.getParam("lower_h_",lower_h);
	nh.getParam("lower_s_",lower_s);
	nh.getParam("lower_v_",lower_v);

	upper_back_hsv = Scalar(upper_h, upper_s, upper_v);
	lower_back_hsv = Scalar(lower_h, lower_s, lower_v);

	Mat img_hsv;
	Mat back_hsv_mask;
	Mat back_hsv;

	cvtColor(raw_image, img_hsv, COLOR_BGR2HSV);
	inRange(img_hsv, lower_back_hsv, upper_back_hsv, back_hsv_mask);
	bitwise_and(raw_img, raw_img, back_hsv, back_hsv_mask);
	cvt_color(back_hsv, back_hsv, COLOR_BGR2GRAY);
	threshold(back_hsv, filtered_img, 150, 255, cv::THRESH_BINARY);
	imshow("filtered_img", filtered_img);

}
