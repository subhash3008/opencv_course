#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//////////// Resize and Crop ///////////// 

int main() {
	string path{ "Resources/test.png" };
	Mat img = imread(path);
	Mat imgResize, imgCrop;
	// cout << img.size() << endl;

	resize(img, imgResize, Size(), 0.8, 0.5);
	Rect roi(100, 100, 300, 250);
	imgCrop = img(roi);

	imshow("Image", img);
	imshow("Image resize", imgResize);
	imshow("Image Crop", imgCrop);
	waitKey(0);

	return 0;
}