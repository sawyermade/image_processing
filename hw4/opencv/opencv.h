#ifndef OPENCV_H
#define OPENCV_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

class opencv {

	private:
		Mat src, dst;


	public:
		//constructor
		opencv(string& fname);

		//operators
		opencv& operator=(string& fname);

		//functions
		void sobelgs(int thresh, pair<int,int> start, pair<int,int> size);
		void sobelhsv(int thresh, pair<int,int> start, pair<int,int> size);
		int checkVal(int val);
		void save(string& outfile);
		void histEqual(pair<int,int> start, pair<int,int> size);
		void histEqualRGB(pair<int,int> start, pair<int,int> size);
		void cannygs(int thresh, pair<int,int> start, pair<int,int> size, int ratio = 3);
};

#endif