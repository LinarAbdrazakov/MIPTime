#include "window.h"


Window::Window(): name("MIPTime") {}
Window::Window(std::string name): name(name) {}

void Window::open() {
	cv::namedWindow("MIPTime", cv::WINDOW_AUTOSIZE);
}

int Window::show(cv::Mat frame) {
	cv::imshow("MIPTime", frame);
	return cv::waitKey(10);
}

int Window::show_jpeg(std::vector <uchar> buff) {
	cv::Mat frame = cv::imdecode(buff, CV_LOAD_IMAGE_COLOR);
    cv::imshow("MIPTime", frame);
	return cv::waitKey(10);
}

void Window::close() {
    cv::destroyWindow("MIPTime");
}