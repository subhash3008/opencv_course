#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/////////////// CAMSCANNER ////////////////
Mat imgOriginal, imgThreshold, imgGray, imgBlur, imgDil, imgErode, imgCanny, imgWarp, imgCrop;
vector<Point> initialPoints, docPoints;

float w = 420, h = 596;

Mat& preprocessing(Mat& imgOriginal) {
	cvtColor(imgOriginal, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	return imgCanny;
}

vector<Point> getContours(Mat& imgThre) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> conPoly(contours.size());

	vector<Point> biggest;
	int maxArea{ 0 };

	for (int i{ 0 }; i < contours.size(); ++i) {
		int area = contourArea(contours[i]);
		cout << "Area : " << area << endl;
		if (area > 1000) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], .02 * peri, true);

			if (area > maxArea && conPoly[i].size() == 4) {
				// drawContours(imgOriginal, conPoly, i, Scalar(255, 255, 0), 2);
				maxArea = area;
				biggest = { conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3] };
			}
		}
	}

	return biggest;
}

void drawPoints(vector<Point> points, Scalar color) {
	for (int i{ 0 }; i < points.size(); ++i) {
		circle(imgOriginal, points[i], 5, color, FILLED);
		putText(imgOriginal, to_string(i), points[i], FONT_HERSHEY_PLAIN, 3, color, 2);
	}
}

vector<Point> reorderPoints(vector<Point> initialPoints) {
	vector<Point> newPoints;
	vector<int> sumPoints, subPoints;

	for (int i{ 0 }; i < 4; ++i) {
		sumPoints.push_back(initialPoints[i].x + initialPoints[i].y);
		subPoints.push_back(initialPoints[i].x - initialPoints[i].y);
	}

	newPoints.push_back(initialPoints[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
	newPoints.push_back(initialPoints[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	newPoints.push_back(initialPoints[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	newPoints.push_back(initialPoints[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
	return newPoints;
}

Mat& getWarp(Mat& img, vector<Point> docPoints, float w, float h) {
	Point2f src[4] = { docPoints[0], docPoints[1], docPoints[2], docPoints[3] };
	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };

	Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));
	return imgWarp;
}

int main() {
	string path{ "Resources/paper.jpg" };
	imgOriginal = imread(path);

	resize(imgOriginal, imgOriginal, Size(), .5, .5);

	// Preprocessing
	imgThreshold = preprocessing(imgOriginal);

	// Get Contours - Biggest
	initialPoints = getContours(imgThreshold);
	// drawPoints(initialPoints, Scalar(0, 0, 255));
	docPoints = reorderPoints(initialPoints);
	// drawPoints(docPoints, Scalar(0, 255, 0));

	// Warp the image
	imgWarp = getWarp(imgOriginal, docPoints, w, h);

	// Crop
	Rect roi(5, 5, w - 10, h - 10);
	imgCrop = imgWarp(roi);


	imshow("Img", imgOriginal);
	imshow("Img Warp", imgWarp);
	imshow("Img Crop", imgCrop);
	waitKey(0);
	return 0;
}