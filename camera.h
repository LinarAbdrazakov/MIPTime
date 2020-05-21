#pragma once

#include <vector>
#include "opencv2/opencv.hpp"


// Class for camera
class Camera {
private:
	cv::VideoCapture cap;

public:
	Camera();
	int open();
	cv::Mat get_frame();
	std::vector <unsigned char> get_jpeg_frame();
};