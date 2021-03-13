#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/////////////// VIRTUAL PAINT ///////////////

VideoCapture cap(0);
Mat img;
vector<vector<int>> newPoints;

vector<vector<int>> myColors{
	{123, 179, 55, 215, 123, 255}, // Orange
	{33, 67, 81, 213, 88, 182}, // Chamkeela
};

vector<Scalar> myColorValues{
	{0, 69, 255},
	{0, 255, 127}
};

Point getContours(const Mat& imgMask) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgMask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);


	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	Point myPoint(0, 0);

	for (int i{ 0 }; i < contours.size(); ++i) {
		int area = contourArea(contours[i]);
		cout << area << endl;

		if (area > 500) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], .02 * peri, true);

			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
			myPoint.x = boundRect[i].x + (boundRect[i].width / 2);
			myPoint.y = boundRect[i].y;

			// drawContours(img, conPoly, i, Scalar(0, 0, 0), 2);
			// rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 255), 5);
		}
	}
	return myPoint;
}

vector<vector<int>> findColor(Mat& img) {
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i{ 0 }; i < myColors.size(); ++i) {
		Scalar lower(myColors[i][0], myColors[i][2], myColors[i][4]);
		Scalar upper(myColors[i][1], myColors[i][3], myColors[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		Point myPoint{ getContours(mask) };
		if (myPoint.x != 0 && myPoint.y != 0) {
			newPoints.push_back({ myPoint.x, myPoint.y, i });
		}
	}
	return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues) {
	for (int i{ 0 }; i < newPoints.size(); ++i) {
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 5, myColorValues[newPoints[i][2]], FILLED);
	}

}

int main() {
	while (1) {
		cap.read(img);

		vector<vector<int>> newpoints{ findColor(img) };
		drawOnCanvas(newPoints, myColorValues);

		imshow("Webcam", img);
		waitKey(1);
	}

	return 0;
}