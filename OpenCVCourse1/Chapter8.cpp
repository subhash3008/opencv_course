#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//////////////// FACE DETECTION /////////////////

int main() {
	string path{ "Resources/test.png" };
	Mat img{ imread(path) };

	CascadeClassifier facecascade;
	facecascade.load("Resources/haarcascade_frontalface_default.xml");

	if (facecascade.empty()) {
		cout << "XML FILE NOT LOADED" << endl;
	}

	vector<Rect> faces;
	facecascade.detectMultiScale(img, faces, 1.1, 10);

	for (int i{ 0 }; i < faces.size(); ++i) {
		rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 255, 0), 2);
	}

	imshow("Img", img);
	waitKey(0);
	return 0;
}