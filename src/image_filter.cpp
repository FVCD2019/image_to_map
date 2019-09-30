#include <image_to_map/ImageFilter.h>

using namespace std;
using namespace cv;

void ImageFilter::backfilter(cv::Mat raw_image, cv::Mat &filtered_img, int upper_h, int upper_s, int upper_v, int lower_h, int lower_s, int lower_v)
{


	upper_back_hsv = Scalar(upper_h, upper_s, upper_v);
	lower_back_hsv = Scalar(lower_h, lower_s, lower_v);

	Mat img_hsv;
	Mat back_hsv_mask;
	Mat back_hsv;

	cvtColor(raw_image, img_hsv, COLOR_BGR2HSV);
	inRange(img_hsv, lower_back_hsv, upper_back_hsv, back_hsv_mask);
	bitwise_and(raw_image, raw_image, back_hsv, back_hsv_mask);
	cvtColor(back_hsv, back_hsv, COLOR_BGR2GRAY);
	threshold(back_hsv, filtered_img, 150, 255, cv::THRESH_BINARY);
	cout << "img_f" << endl;
	imshow("filtered_img", filtered_img);

}
