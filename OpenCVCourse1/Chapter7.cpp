#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;


/////////// DETECTING SHAPES ////////////

void getContours(const Mat& imgDil, Mat& img) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); ++i) {
		int area = contourArea(contours[i]);
		cout << area << endl;
		string objectType;
		if (area >= 1000) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], .02 * peri, true);
			cout << "Points Count" << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
			int objCorner = static_cast<int>(conPoly[i].size());
			if (objCorner == 3) {
				objectType = "Triangle";
			} else if (objCorner == 4) {
				objectType = "Rectangle";
				float ratio = (static_cast<float>(boundRect[i].width) / static_cast<float>(boundRect[i].height));
				if (ratio > .95 && ratio < 1.05) {
					objectType = "Square";
				}
			} else {
				objectType = "Circle";
			}

			drawContours(img, conPoly, i, Scalar(255, 255, 0), 2);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 255), 2);
			putText(img, objectType, Point(boundRect[i].x, boundRect[i].y - 5), FONT_HERSHEY_SIMPLEX, .5, Scalar(0, 0, 0), 1);

		}
	}
}

int main() {
	string path{ "Resources/shapes.png" };
	Mat img{ imread(path) };
	Mat imgGray, imgBlur, imgCanny, imgDil;

	// Preprocessing of image
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel{ getStructuringElement(MORPH_RECT, Size(3, 3)) };
	dilate(imgCanny, imgDil, kernel);

	getContours(imgDil, img);

	//imshow("Image Cany", imgCanny);
	//imshow("Image Dilated", imgDil);
	imshow("Image", img);
	waitKey(0);
	return 0;
}