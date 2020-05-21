#include "window.h"


Window::Window(): name("MIPTime") {}
Window::Window(std::string name): name(name) {}

void Window::open() {
	cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
}

int Window::show(cv::Mat frame, int wait=10) {
	cv::imshow(name, frame);
	return cv::waitKey(wait);
}

int Window::show_jpeg(std::vector <unsigned char> buff, int wait=10) {
	cv::Mat frame = cv::imdecode(buff, CV_LOAD_IMAGE_COLOR);
    cv::imshow(name, frame);
	return cv::waitKey(wait);
}

void Window::close() {
    cv::destroyWindow(name);
}