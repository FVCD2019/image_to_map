#include <image_to_map/ImageFilter.h>

using namespace std;
using namespace cv;

void ImageFilter::backfilter(cv::Mat raw_image, cv::Mat &filtered_img, int upper_h, int upper_s, int upper_v, int lower_h, int lower_s, int lower_v)
{


	upper_back_hsv = Scalar(upper_h, upper_s, upper_v);
	lower_back_hsv = Scalar(lower_h, lower_s, lower_v);
	upper_back_rgb = Scalar(175, 175, 175);
	lower_back_rgb = Scalar(105, 105, 105);

	Mat img_hsv;
	Mat hsv_blur;
	Mat back_rgb_mask;
	Mat back_rgb;
	Mat back_hsv;

	cvtColor(raw_image, img_hsv, COLOR_BGR2HSV);
	blur(img_hsv, img_hsv, Size(9,9));
	inRange(img_hsv, lower_back_hsv, upper_back_hsv, filtered_img);
//	inRange(raw_image, lower_back_rgb, upper_back_rgb, back_rgb_mask);
//	threshold(back_rgb_mask, filtered_img, 160, 255, cv::THRESH_BINARY);
	imshow("filtered_img", filtered_img);
	waitKey(3);

}
