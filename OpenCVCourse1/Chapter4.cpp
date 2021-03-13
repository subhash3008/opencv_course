#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//////// Draw Shapes and Text //////////

int main() {
	// Blank image
	Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));
	circle(img, Point(256, 256), 155, Scalar(0, 0, 0), FILLED);
	rectangle(img, Point(130, 266), Point(382, 286), Scalar(255, 255, 0), FILLED);
	line(img, Point(130, 296), Point(382, 296), Scalar(255, 255, 0), 2);

	putText(img, "Subhash Test", Point(137, 262), FONT_HERSHEY_SIMPLEX, .75, Scalar(255, 255, 0), 1);

	imshow("Image", img);
	waitKey(0);

	return 0;
}