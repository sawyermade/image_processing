#include "utilities.h"

#define MAXRGB 255
#define MINRGB 0

#define WHITE 255
#define BLACK 0

std::string utilities::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utilities::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	else
		return value;
}


//MY SHIT
void utilities::roiThresh(image &src, image &tgt, pair<int, int> startPoints, pair<int, int> sizexy, int thresh) {

	//
	pair<int, int> endPoints;
	int pix;

	//
	endPoints.first = startPoints.first + sizexy.first;
	endPoints.second = startPoints.second + sizexy.second;

	//
	for(int i = startPoints.first; i < endPoints.first; ++i)
		for(int j = startPoints.second; j < endPoints.second; ++j) {
			
			pix = src.getPixel(i,j);

			if(pix <= thresh)
				tgt.setPixel(i,j,WHITE);
			else
				tgt.setPixel(i,j,BLACK);
			//tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+thresh));
		}
}


//
void utilities::roiBinarizeColor(image &src, image &tgt, int threshold, int colors[], pair<int, int> startPoints, pair<int, int> sizexy)
{
	//
	int dist;
	pair<int, int> endPoints;

	//
	endPoints.first = startPoints.first + sizexy.first;
	endPoints.second = startPoints.second + sizexy.second;

	//
	for (int i = startPoints.first; i < endPoints.first; i++)
		for (int j = startPoints.second; j < endPoints.second; j++)
		{

			dist = sqrt(pow(src.getPixel(i,j,0) - colors[0],2) + pow(src.getPixel(i,j,1) - colors[1],2) + pow(src.getPixel(i,j,2) - colors[2],2));

			if(dist <= threshold) {
				tgt.setPixel(i,j,0,WHITE);
				tgt.setPixel(i,j,1,WHITE);
				tgt.setPixel(i,j,2,WHITE);
			}

			else {
				tgt.setPixel(i,j,0,BLACK);
				tgt.setPixel(i,j,1,BLACK);
				tgt.setPixel(i,j,2,BLACK);
			}
		}	
}


//
void utilities::roiSmooth1D(image& src, image& tgt, int ws, pair<int, int> startPoints, pair<int, int> size) {

	int avg, m = (ws-1)/2;
	image intermediate;
	pair<int, int> endPoints;

	//copies image and calculates endpoints.
	intermediate.copyImage(src);
	endPoints.first = startPoints.first + size.first;
	endPoints.second = startPoints.second + size.second;

	//Horizontal 1D
	for(int i = startPoints.first + m; i < endPoints.first - m; ++i) {

		avg = 0;
		for(int j = startPoints.second + m; j < endPoints.second - m; ++j) {

			if(j == startPoints.second + m) {

				//count = 0;
				for(int k = j-m; k <= j+m; ++k) {
					avg += src.getPixel(i,k);
					//++count;
				}
				//cout << count << " ";
				intermediate.setPixel(i,j,avg/ws);
			}

			else {

				avg = avg - src.getPixel(i,j-1-m) + src.getPixel(i,j+m);
				intermediate.setPixel(i,j,avg/ws);
			}

		}
	}


	//Vertical 1D
	for(int j = startPoints.second + m; j < endPoints.second - m; ++j) {

		avg = 0;
		for(int i = startPoints.first + m; i < endPoints.first - m; ++i) {

			//int count = 0;
			if(i == startPoints.first + m) {

				for(int k = i-m; k <= i+m; ++k) {
					avg += intermediate.getPixel(k,j);
					//++count;
				}
				//cout << count << " ";
				tgt.setPixel(i,j,avg/ws);
			}

			else {

				avg = avg - intermediate.getPixel(i-1-m, j) + intermediate.getPixel(i+m,j);
				tgt.setPixel(i,j,avg/ws);
			}
		}
	}

}


//
void utilities::roiSmooth2D(image& src, image& tgt, int ws, pair<int, int> startPoints, pair<int, int> size) {

	pair<int, int> endPoints;
	int avg, m = (ws-1)/2;

	endPoints.first = startPoints.first + size.first;
	endPoints.second = startPoints.second + size.second;

	for(int i = startPoints.first + m; i < endPoints.first - m; ++i) {
		
		for(int j = startPoints.second + m; j < endPoints.second - m; ++j) {

			avg = 0;
			for(int k = i-m; k <= i+m; ++k)
				for(int l = j-m; l <= j+m; ++l)
					avg += src.getPixel(k,l);
			
			//
			avg /= (ws*ws);
			tgt.setPixel(i,j,avg);
		}
	}
}


// void utilities::binarizeColor(image &src, image &tgt, int threshold, int colors[])
// {
// 	int dist;

// 	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

// 	for (int i=0; i<src.getNumberOfRows(); i++)
// 		for (int j=0; j<src.getNumberOfColumns(); j++)
// 		{

// 			dist = sqrt(pow(src.getPixel(i,j,0) - colors[0],2)+pow(src.getPixel(i,j,1) - colors[1],2)+pow(src.getPixel(i,j,2) - colors[2],2));

// 			if(dist <= threshold) {
// 				tgt.setPixel(i,j,0,WHITE);
// 				tgt.setPixel(i,j,1,WHITE);
// 				tgt.setPixel(i,j,2,WHITE);
// 			}

// 			else {
// 				tgt.setPixel(i,j,0,BLACK);
// 				tgt.setPixel(i,j,1,BLACK);
// 				tgt.setPixel(i,j,2,BLACK);
// 			}
// 		}	
// }


// void utilities::roiAddGrey(image &src, image &tgt, int value, pair<int, int> startPoints, pair<int, int> endPoints)
// {
// 	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
// 	for (int i = 0; i < src.getNumberOfRows(); i++)
// 		for (int j = 0; j < src.getNumberOfColumns(); j++)
// 		{
// 			if( (i >= startPoints.first && i < endPoints.first) && (j >= startPoints.second && j < endPoints.second) ) {
// 				tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value));
// 			}

// 			else
// 				tgt.setPixel(i,j,checkValue(src.getPixel(i,j)));

			
// 		}
// }


// void utilities::roiBinarize(image &src, image &tgt, int threshold)
// {
// 	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
// 	for (int i=0; i<src.getNumberOfRows(); i++)
// 	{
// 		for (int j=0; j<src.getNumberOfColumns(); j++)
// 		{
// 			if (src.getPixel(i,j) < threshold)
// 				tgt.setPixel(i,j,MINRGB);
// 			else
// 				tgt.setPixel(i,j,MAXRGB);
// 		}
// 	}
// }