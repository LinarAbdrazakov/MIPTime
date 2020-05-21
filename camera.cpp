#include "camera.h"


Camera::Camera() {
}

int Camera::open() {
	if(!cap.open(0)) return -1;
	return 0;
}

cv::Mat Camera::get_frame() {
	cv::Mat frame;
	cap >> frame;

	return frame;
}

std::vector <unsigned char> Camera::get_jpeg_frame() {
	cv::Mat frame = get_frame();

	std::vector <unsigned char> buff;                  //buffer for coding
	std::vector <int> param(2);
	param[0] = cv::IMWRITE_JPEG_QUALITY;
	param[1] = 20;                             //default(95) 0-100
	cv::imencode(".jpg", frame, buff, param);

	return buff;
}