#pragma once

#include <vector>
#include <string>
#include "opencv2/opencv.hpp"


class Window {
private:
	std::string name;

public:
	Window();
	Window(std::string);

	void open();
	int show(cv::Mat frame);	
	int show_jpeg(std::vector <uchar> buff);
	void close();
};