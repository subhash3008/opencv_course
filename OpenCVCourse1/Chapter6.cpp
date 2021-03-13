#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

///////////// COLOR DETECTION ///////////

Mat imgHSV, mask;
int hmin{ 0 }, smin{ 110 }, vmin{ 253 };
int hmax{ 19 }, smax{ 240 }, vmax{ 255 };

int main() {
	string path{ "Resources/lambo.png" };
	Mat img{ imread(path) };

	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	namedWindow("Trackbars", (640, 240));
	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 255);

	while (true) {
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);

		inRange(imgHSV, lower, upper, mask);

		imshow("Image", img);
		imshow("Image HSV", imgHSV);
		imshow("Mask", mask);
		waitKey(1);
	}
	return 0;
}