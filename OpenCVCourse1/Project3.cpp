#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

/////////////////// Number Plate Detection /////////////////

int main() {
	VideoCapture cap(0);
	Mat img;

	CascadeClassifier plateCascade;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");

	if (plateCascade.empty()) {
		cout << "XML FILE NOT LOADED" << endl;
	}

	vector<Rect> plates;

	while (true) {
		cap.read(img);

		plateCascade.detectMultiScale(img, plates, 1.1, 10);

		for (int i{ 0 }; i < plates.size(); ++i) {
			Mat imgCrop = img(plates[i]);
			//imshow(to_string(i), imgCrop);
			imwrite("Resources/Plates/" + to_string(i) + ".png", imgCrop);
			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(0, 255, 0), 2);
		}

		imshow("Img", img);
		waitKey(1);
	}

	return 0;
}