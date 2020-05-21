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
	int show(cv::Mat frame, int wait);	
	int show_jpeg(std::vector <unsigned char> buff, int wait);
	void close();
};