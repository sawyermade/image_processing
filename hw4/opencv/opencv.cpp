#include "opencv.h"
#include <cmath>

opencv::opencv(string& fname) {

	//src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
	src = imread(fname);
	if(!src.data) {
		cout << "\nError reading file " << fname << endl;
		return;
	}

	dst = src.clone();
}


opencv& opencv::operator=(string& fname) {

	src = imread(fname);
	if(!src.data) {
		cout << "\nError reading file " << fname << endl;
		return *this;
	}

	dst = src.clone();

	return *this;
}


void opencv::sobelgs(int thresh, pair<int,int> start, pair<int,int> size) {

	Mat dst_roi;
	Mat grad_x, grad_y;
	Rect roi(start.first, start.second, size.first, size.second);
	int ddepth = -1, dx, dy, ksize = 3;

	//Roi and conversion to grayscale.
	dst_roi = src(roi);
	cvtColor(dst_roi, dst_roi, CV_BGR2GRAY);
	
	//Gets Gradient X
	dx = 1, dy = 0;
	Sobel(dst_roi, grad_x, ddepth, dx, dy, ksize);

	//Gets Gradient Y
	dx = 0, dy = 1;
	Sobel(dst_roi, grad_y, ddepth, dx, dy, ksize);

	//Calculates Gradient for Both
	for(int i = 0; i < dst_roi.rows; ++i) {
		for(int j = 0; j < dst_roi.cols; ++j) {

			dx = grad_x.at<uchar>(i,j);
			dy = grad_y.at<uchar>(i,j);

			dst_roi.at<uchar>(i,j) = checkVal(sqrt(dx*dx + dy*dy));
		}
	}
	//Sobel(dst_roi,dst_roi,-1,1,1,3);

	//threshold if thresh value is in range.
	if(thresh >= 0 && thresh <= 255)
		threshold(dst_roi, dst_roi, thresh, 255, THRESH_BINARY);

	//saves gradient or thresholded gradient image, depending on value of thresh above.
	cvtColor(dst_roi, dst_roi, CV_GRAY2BGR);
	dst_roi.copyTo(dst(roi));
	// imshow("Gradient Both", dst);
	// waitKey(0);
}


void opencv::sobelhsv(int thresh, pair<int,int> start, pair<int,int> size) {

	Mat dst_roi;
	Mat grad_x, grad_y;
	Mat channels[3];
	Rect roi(start.first, start.second, size.first, size.second);
	int ddepth = -1, dx, dy, ksize = 3;

	//Roi and conversion to hsv.
	dst_roi = src(roi);
	cvtColor(dst_roi, dst_roi, CV_BGR2HSV);

	//splits into channels
	split(dst_roi, channels);
	
	//Gets Gradient X
	dx = 1, dy = 0;
	Sobel(channels[2], grad_x, ddepth, dx, dy, ksize);

	//Gets Gradient Y
	dx = 0, dy = 1;
	Sobel(channels[2], grad_y, ddepth, dx, dy, ksize);

	//Calculates Gradient for Both
	for(int i = 0; i < dst_roi.rows; ++i) {
		for(int j = 0; j < dst_roi.cols; ++j) {

			dx = grad_x.at<uchar>(i,j);
			dy = grad_y.at<uchar>(i,j);

			channels[2].at<uchar>(i,j) = checkVal(sqrt(dx*dx + dy*dy));

			channels[0].at<uchar>(i,j) = 0;
			channels[1].at<uchar>(i,j) = 0;
		}
	}

	//threshold if thresh value is in range.
	if(thresh >= 0 && thresh <= 255)
		threshold(channels[2], channels[2], thresh, 255, THRESH_BINARY);

	merge(channels, 3, dst_roi);

	//saves gradient or thresholded gradient image, depending on value of thresh above.
	cvtColor(dst_roi, dst_roi, CV_HSV2BGR);
	dst_roi.copyTo(dst(roi));
	// imshow("Gradient Both", dst);
	// waitKey(0);
}


int opencv::checkVal(int val) {

	if(val > 255)
		return 255;

	if(val < 0)
		return 0;

	return val;
}


void opencv::save(string& outfile) {

	imwrite(outfile,dst);
}


void opencv::histEqual(pair<int,int> start, pair<int,int> size) {

	Mat dst_roi;
	Mat channels[3];
	Rect roi(start.first, start.second, size.first, size.second);
	
	//sets roi and converts to HSV/HSI.
	dst_roi = src(roi);
	cvtColor(dst_roi, dst_roi, CV_BGR2HSV);

	//splits channels and equalizes the V/I channel and then merges back.
	split(dst_roi, channels);
	equalizeHist(channels[2], channels[2]);
	merge(channels, 3, dst_roi);
	
	//converts back to RGB and saves roi to dst.
	cvtColor(dst_roi, dst_roi, CV_HSV2BGR);
	dst_roi.copyTo(dst(roi));
	// imshow("test", dst_roi);
	// waitKey(0);
}


void opencv::histEqualRGB(pair<int,int> start, pair<int,int> size) {

	Mat dst_roi;
	Rect roi(start.first, start.second, size.first, size.second);
	Mat channels[3];

	dst_roi = src(roi);

	split(dst_roi, channels);

	equalizeHist(channels[0], channels[0]);
	equalizeHist(channels[1], channels[1]);
	equalizeHist(channels[2], channels[2]);

	merge(channels, 3, dst_roi);

	dst_roi.copyTo(dst(roi));
}


void opencv::cannygs(int thresh, pair<int,int> start, pair<int,int> size, int ratio) {

	Mat dst_roi;
	Rect roi(start.first, start.second, size.first, size.second);

	dst_roi = src(roi);

	cvtColor(dst_roi, dst_roi, CV_BGR2GRAY);

	Canny(dst_roi, dst_roi, thresh, thresh*ratio);

	cvtColor(dst_roi, dst_roi, CV_GRAY2BGR);

	dst_roi.copyTo(dst(roi));
}