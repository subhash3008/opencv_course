#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//////////// Warp Images ////////////
float w{ 250 };
float h{ 350 };

Mat matrix, imgWarp;

int main() {
	string path{ "Resources/cards.jpg" };
	Mat img = imread(path);

	Point2f src[4]{ {529, 144}, {772, 192}, {403, 394}, {677, 459} };
	Point2f dst[4]{ {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };

	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	for (int i = 0; i < 4; ++i) {
		circle(img, src[i], 5, Scalar(0, 0, 255), FILLED);
	}

	imshow("Image", img);
	imshow("Image warp", imgWarp);
	waitKey(0);
	return 0;
}