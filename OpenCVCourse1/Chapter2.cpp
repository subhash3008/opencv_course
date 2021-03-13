#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//////// Basic Functions //////////

int main() {
	string path{ "Resources/test.png" };
	Mat img = imread(path);
	Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;

	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(img, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	erode(imgDil, imgErode, kernel);


	imshow("Image", img);
	imshow("Image Gray", imgGray);
	imshow("Image Blur", imgBlur);
	imshow("Canny Image", imgCanny);
	imshow("Image Dilation", imgDil);
	imshow("Image Erosion", imgErode);
	waitKey(0);
	return 0;
}