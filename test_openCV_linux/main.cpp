#include <stdio.h>
#include <opencv2/opencv.hpp>

int main(int argc, char* args[])
{
    cv::Mat image = cv::imread("/home/sheld/git_repository/Sungsu_Lab/test_openCV_linux/at_kananaskis.jpg");
    printf("image data=%s\r\n", image.data ? "YES" : "NO");

    cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Image", image);
    cv::waitKey(0);

    return 0;
}