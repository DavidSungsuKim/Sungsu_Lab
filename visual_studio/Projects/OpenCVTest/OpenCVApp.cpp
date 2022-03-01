#include "pch.h"
#include "OpenCVApp.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;

COpenCVApp::COpenCVApp()
{

}

COpenCVApp::~COpenCVApp()
{

}

void COpenCVApp::Test(void)
{
	cv::Mat image;

	image = cv::imread("D:/at kananaskis.jpg");
	if (image.empty())
		return;

	string windowName = "First window";
	cv::namedWindow(windowName);
	cv::imshow(windowName, image);
	cv::waitKey(0);
}