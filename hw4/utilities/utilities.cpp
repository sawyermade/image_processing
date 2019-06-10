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

/*-----------------------------------------------------------------------*/
//MY SHIT
/*-----------------------------------------------------------------------*/

//Binarize/Thresholding for grayscale. Runs at O(n^2).
void utilities::roiBinarize(image &src, image &tgt, pair<int, int> startPoints, pair<int, int> sizexy, int thresh) {

	//pair for end points x and y. first is x, second is y.
	pair<int, int> endPoints;

	//calculates end points using start points x y and size for x y.
	endPoints.first = startPoints.first + sizexy.first;
	endPoints.second = startPoints.second + sizexy.second;

	//runs through all pixels in ROI and if pixel value less than or equal to threshold it tunrs it white, else black.
	for(int i = startPoints.first; i < endPoints.first; ++i) {
		for(int j = startPoints.second; j < endPoints.second; ++j) {

			if(src.getPixel(i,j) <= thresh)
				tgt.setPixel(i,j,WHITE);
			else
				tgt.setPixel(i,j,BLACK);
		}
	}
}


//Binarize/Thresholding for Coloor images. Runs at O(n^2).
void utilities::roiBinarizeColor(image &src, image &tgt, int threshold, int colors[], pair<int, int> startPoints, pair<int, int> sizexy)
{
	//variables for color distance and end points for ROI.
	int dist;
	pair<int, int> endPoints;

	//calculates end points using start points and sizes.
	endPoints.first = startPoints.first + sizexy.first;
	endPoints.second = startPoints.second + sizexy.second;

	//runs through all the pixels in the ROI, calculates color distance and
	//sets pixel distances that are less than or equal to ThresholdColor(TC) to white, the rest to black.
	for (int i = startPoints.first; i < endPoints.first; i++) {
		for (int j = startPoints.second; j < endPoints.second; j++) {

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
}



//1D Adaptive Window Smooth. Runs at O(2n^2 + 2m) where m << n, so runs at O(n^2).
void utilities::roiSmooth1DAdaptive(image& src, image& tgt, int ws, pair<int, int> start, pair<int, int> size) {

	int avg, avgg, avgb, k, m = (ws-1)/2;
	image intermediate;
	pair<int, int> end;

	//copies image and calculates endpoints.
	intermediate = src;
	end.first = start.first + size.first;
	end.second = start.second + size.second;

	//cout << "\nFunction: 1DSmoothAdaptive" << endl; //debug

	//Horizontal 1D
	for(int i = start.first; i < end.first; ++i) {

		avg = 0, avgg = 0, avgb = 0;
		k = start.second;
		for(int j = start.second; j < end.second; ++j) {

			if(j <= start.second + m) {
		
				for(; k <= j+m; ++k) {
					avg += src.getPixel(i,k);
					avgg += src.getPixel(i,k,GREEN);
					avgb += src.getPixel(i,k,BLUE);
					//++count; //debug
				}
				//cout << count << " "; //debug
				intermediate.setPixel(i,j,checkValue(avg/(k - start.second)));
				intermediate.setPixel(i,j,GREEN,checkValue(avgg/(k - start.second)));
				intermediate.setPixel(i,j,BLUE,checkValue(avgb/(k - start.second)));
			}

			else if(j >= end.second - m) {

				avg -= src.getPixel(i,j-1-m);
				avgg -= src.getPixel(i,j-1-m,GREEN);
				avgb -= src.getPixel(i,j-1-m,BLUE);

				intermediate.setPixel(i,j,checkValue(avg/(end.second - j + m)));
				intermediate.setPixel(i,j,GREEN,checkValue(avgg/(end.second - j + m)));
				intermediate.setPixel(i,j,BLUE,checkValue(avgb/(end.second - j + m)));
			}

			else {

				avg = avg - src.getPixel(i,j-1-m) + src.getPixel(i,j+m);
				avgg = avgg - src.getPixel(i,j-1-m,GREEN) + src.getPixel(i,j+m,GREEN);
				avgb = avgb - src.getPixel(i,j-1-m,BLUE) + src.getPixel(i,j+m,BLUE);

				intermediate.setPixel(i,j,checkValue(avg/ws));
				intermediate.setPixel(i,j,GREEN,checkValue(avgg/ws));
				intermediate.setPixel(i,j,BLUE,checkValue(avgb/ws));
			}

		}
	}

	//Vertical 1D
	for(int j = start.second; j < end.second; ++j) {

		avg = 0, avgg = 0, avgb = 0;
		k = start.first;
		for(int i = start.first; i < end.first; ++i) {
			
			if(i <= start.first + m) {

				for(; k <= i+m; ++k) {
					avg += intermediate.getPixel(k,j);
					avgg += intermediate.getPixel(k,j,GREEN);
					avgb += intermediate.getPixel(k,j,BLUE);
				}
				tgt.setPixel(i,j,checkValue(avg/(k - start.first)));
				tgt.setPixel(i,j,GREEN,checkValue(avgg/(k - start.first)));
				tgt.setPixel(i,j,BLUE,checkValue(avgb/(k - start.first)));
			}

			else if(i >= end.first - m) {

				avg -= intermediate.getPixel(i-1-m,j);
				avgg -= intermediate.getPixel(i-1-m,j,GREEN);
				avgb -= intermediate.getPixel(i-1-m,j,BLUE);

				tgt.setPixel(i,j,checkValue(avg/(end.first - i + m)));
				tgt.setPixel(i,j,GREEN,checkValue(avgg/(end.first - i + m)));
				tgt.setPixel(i,j,BLUE,checkValue(avgb/(end.first - i + m)));
			}

			else {

				avg = avg - intermediate.getPixel(i-1-m, j) + intermediate.getPixel(i+m,j);
				avgg = avgg - intermediate.getPixel(i-1-m, j,GREEN) + intermediate.getPixel(i+m,j,GREEN);
				avgb = avgb - intermediate.getPixel(i-1-m, j,BLUE) + intermediate.getPixel(i+m,j,BLUE);

				tgt.setPixel(i,j,checkValue(avg/ws));
				tgt.setPixel(i,j,GREEN,checkValue(avgg/ws));
				tgt.setPixel(i,j,BLUE,checkValue(avgb/ws));
			}
		}
	}
}

//2D Adaptive Window Smooth. Runs at O(n^2 * 2m^2) = O(n^2 * m^2).
void utilities::roiSmooth2DAdaptive(image& src, image& tgt, int ws, pair<int, int> start, pair<int, int> size) {

	pair<int, int> end;
	int m = (ws-1)/2, k, l, limk, liml, wk, wl;

	int avgr, avgg, avgb;

	end.first = start.first + size.first;
	end.second = start.second + size.second;

	for(int i = start.first; i < end.first; ++i) {

		//sets window x start point.
		if(i - start.first < m) {
			k = start.first;
		}
		else {
			k = i - m;
		}
		
		//sets windows x limit.
		if(end.first - i <= m) {
			limk = i + (end.first - i - 1);
		}
		else {
			limk = i + m;
		}
		wk = limk - k + 1;

		//cout << "i:" << i << " "; //debug
		//cout << "k:" << k << "-lmk:" << limk << " "; //debug

		for(int j = start.second; j < end.second; ++j) {

			//sets window y start.
			if(j - start.second < m) {
				l = start.second;
			}
			else {
				l = j - m;
			}
			
			//sets window y limit.
			if(end.second - j <= m) {
				liml = j + (end.second - j - 1);
			}
			else {
				liml = j + m;
			}
			wl = liml - l + 1;

			//cout << "l:" << l << "-lml:" << liml << " "; //debug

			//calculates the windows pixel average for rgb.
			avgr = 0, avgg = 0, avgb = 0;
			for(int x = k; x <= limk; ++x) {
				//cout << "\nin loop "; //debug
				for(int y = l; y <= liml; ++y) {

					//cout << "\nin dbl loop "; //debug

					avgr += src.getPixel(x,y,RED);
					avgg += src.getPixel(x,y,GREEN);
					avgb += src.getPixel(x,y,BLUE);
				}
			}
			
			
			//cout << avgr << ":" << avgg << ":" << avgb << " "; //debug

			//sets target image's new pixel values for i,j.
			tgt.setPixel(i,j,RED,checkValue(avgr/(wk*wl)));
			tgt.setPixel(i,j,GREEN,checkValue(avgg/(wk*wl)));
			tgt.setPixel(i,j,BLUE,checkValue(avgb/(wk*wl)));
		}
	}
}


/********** PJ2 *************************/

//
void utilities::histCreate(image& src, int hist[256], pair<int, int> start, pair<int, int> stop) {
	
	stop.first += start.first, stop.second += start.second;

	for(int i = start.first; i < stop.first; ++i)
		for(int j = start.second; j < stop.second; ++j)
			++hist[src.getPixel(i,j)];
}

//debug
void utilities::histPrint(int hist[256]) {

	int sum = 0;

	for(int i = 0; i < 256; ++i) {
		cout << hist[i] << endl;
		sum += hist[i];
	}

	cout << "\nSUM = " << sum << endl;
}

//
void utilities::histSave(int hist[256], string& fname) {

	//cout << "\nIn HistSave" << endl;

	//normalize hist array;
	int max = 0, min;

	for(int i = 0; i < 256; ++i)
		if(max < hist[i])
			max = hist[i];

	min = max;
	for(int i = 0; i < 256; ++i)
		if(min > hist[i])
			min = hist[i];

	//double ratio = 256.0 / (double)max;


	//tgt.resize(HISTMAX,HISTMAX);
	double ratio;
	image tgt(HISTMAX,HISTMAX,fname);

	//
	for(int j = 0; j < HISTMAX; ++j) {

		ratio = (double)(hist[j] - min) / (double)(max - min);
		//cout << ratio << "  ";
		for(int i = 0; i < HISTMAX; ++i) {

			if(i < 255 - (255 *ratio))
				tgt.setAll(i,j,BLACK);
			else
				tgt.setAll(i,j,WHITE);
		}
	}

	tgt.save();
}

void utilities::histStretchGS(image& src,image& tgt,pair<int,int> ab,pair<int,int> cd,pair<int,int> start,pair<int,int> stop) {


	double ratio, tempd;
	int temp;

	stop.first += start.first;
	stop.second += start.second;

	ratio = ((double)cd.second - (double)cd.first) / ((double)ab.second - (double)ab.first);

	//converts pixels using Inew = (d-c)/(b-a)[Iij - a] + c
	for(int i = start.first; i < stop.first; ++i) {

		for(int j = start.second; j < stop.second; ++j) {
			
			if(src.getPixel(i,j) < ab.first)
				tgt.setPixel(i,j,BLACK);

			else if(src.getPixel(i,j) > ab.second)
				tgt.setPixel(i,j,WHITE);

			else {
				tempd = ratio * (double)(src.getPixel(i,j) - ab.first) + cd.first;
				temp = tempd;
				tgt.setPixel(i,j,checkValue(temp));
			}
		}
	}
}


void utilities::stringToChar(string& str, char* ch) {

	for(int i = 0; i < (int)str.size(); ++i)
		ch[i] = str[i];
	ch[str.size()] = '\0';
}


void utilities::histSave(int hbefore[256],int hafter[256],string& fname) {

	//normalize hist array;
	int maxbef = 0, maxaft = 0, minbef, minaft;

	for(int i = 0; i < 256; ++i) {
		if(maxbef < hbefore[i])
			maxbef = hbefore[i];
		if(maxaft < hafter[i])
			maxaft = hafter[i];
	}

	minbef = maxbef, minaft = maxaft;
	for(int i = 0; i < 256; ++i) {
		if(minbef > hbefore[i])
			minbef = hbefore[i];
		if(minaft > hafter[i])
			minaft = hafter[i];
	}

	//variables for the two histograms before and after.
	int size = 512;
	double ratio, ratioa;
	image tgt(size,size,fname), temp(size,size);

	
	for(int j = 0; j < HISTMAX; ++j) {

		ratio = (double)(hbefore[j] - minbef) / (double)(maxbef - minbef);
		ratioa = (double)(hafter[j] - minaft) / (double)(maxaft - minaft);
		//cout << ratio << "  ";
		for(int i = 0; i < size; ++i) {

			if(i < (size-1) - ((size-1) *ratio)) {
				//tgt.setPixel(i,j,BLACK); //for size = 256

				tgt.setAll(i,j*2,BLACK);
				tgt.setAll(i,j*2+1,BLACK);
			}
			else {
				//tgt.setPixel(i,j,WHITE); //for size = 256

				tgt.setAll(i,j*2,WHITE);
				tgt.setAll(i,j*2+1,WHITE);
			}

			if(i < (size-1) - ((size-1) *ratioa)) {
				//temp.setPixel(i,j,BLACK); //for size = 256

				temp.setAll(i,j*2,BLACK);
				temp.setAll(i,j*2+1,BLACK);
			}
			else {
				//temp.setPixel(i,j,WHITE); //for size = 256

				temp.setAll(i,j*2,WHITE);
				temp.setAll(i,j*2+1,WHITE);
			}
		}
	}

	tgt += temp;
	tgt.save();
}


bool utilities::stretchInBounds(image& src, string& infile, pair<int,int> start, pair<int,int> size, pair<int,int> ab, pair<int,int> cd) {

	bool inboundsa = true, inboundsb = true;

	inboundsa = src.isInbounds(start.first + size.first,start.second + size.second);
	inboundsb = src.isInbounds(start.first,start.second);

	if(inboundsa == false || inboundsb == false) {
		cout << "\nROI out of bounds. Histogram Stretching not processed on " << infile << ". ROI: " << start.first << " "
		<< start.second << " " << size.first << " " << size.second << endl;
		return false;
	}

	// if(ab.first < cd.first || ab.second > cd.second) {
	// 	cout << "\nValues for a or b are smaller than values for c or d. Hist Stretching cannot complete on " << infile << endl;
	// 	cout << "a=" << ab.first << ", b=" << ab.second << ", c=" << cd.first << ", d=" << cd.second << endl;
	// 	return false;
	// }

	if(ab.first < 0 || ab.first > 255 || ab.second < 0 || ab.second > 255 || cd.first < 0 || cd.first > 255 || cd.second < 0 || cd.second > 255) {
		cout << "\nValue a, b, c, or d are out of bounds. Acceptable intervals = [0,255]" << endl;
		cout << "a=" << ab.first << ", b=" << ab.second << ", c=" << cd.first << ", d=" << cd.second << endl;
		return false;
	}

	return true;
}


void utilities::histCreateRGB(image& src, int hist[256], pair<int, int> start , pair<int, int> size, const int COLOR) {

	pair<int,int> stop;

	stop.first = start.first + size.first;
	stop.second = start.second + size.second;

	for(int i = start.first; i < stop.first; ++i)
		for(int j = start.second; j < stop.second; ++j)
			++hist[src.getPixel(i,j,COLOR)];
}


void utilities::histStretchRGB(image& src, image& tgt, pair<int,int> ab, pair<int,int> cd, pair<int,int> start, pair<int,int> size, const int COLOR) {

	double ratio, tempd;
	int temp;
	pair<int,int> stop;

	stop.first = start.first + size.first;
	stop.second = start.second + size.second;

	ratio = (double)(cd.second - cd.first) / (double)(ab.second - ab.first);

	//converts pixels using Inew = (d-c)/(b-a)[Iij - a] + c
	for(int i = start.first; i < stop.first; ++i) {

		for(int j = start.second; j < stop.second; ++j) {
			
			if(src.getPixel(i,j,COLOR) < ab.first)
				tgt.setPixel(i,j,COLOR,BLACK);

			else if(src.getPixel(i,j,COLOR) > ab.second)
				tgt.setPixel(i,j,COLOR,WHITE);

			else {
				tempd = ratio * (double)(src.getPixel(i,j,COLOR) - ab.first) + cd.first;
				temp = tempd;
				tgt.setPixel(i,j,COLOR,checkValue(temp));
			}
		}
	}
}

//overloaded stretches all colors RGB.
void utilities::histStretchRGB(image& src, image& tgt, pair<int,int> ab, pair<int,int> cd, pair<int,int> start, pair<int,int> size) {

	histStretchRGB(src,tgt,ab,cd,start,size,RED);
	histStretchRGB(src,tgt,ab,cd,start,size,GREEN);
	histStretchRGB(src,tgt,ab,cd,start,size,BLUE);
}

//overloaded creates all 3 RGB histograms.
void utilities::histCreateRGB(image& src, int histr[256], int histg[256], int histb[256], pair<int,int> start, pair<int,int> size) {

	histCreateRGB(src,histr,start,size,RED);
	histCreateRGB(src,histg,start,size,GREEN);
	histCreateRGB(src,histb,start,size,BLUE);
}


// void utilities::histCreateRGB(image& src, int hist[256], pair<int, int> start , pair<int, int> size) {

// 	unsigned int avg;
// 	pair<int,int> stop;

// 	stop.first = start.first + size.first;
// 	stop.second = start.second + size.second;

// 	for(int i = start.first; i < stop.first; ++i) {

// 		for(int j = start.second; j < stop.second; ++j) {
			
// 			avg = (double)src.getPixel(i,j,RED)*0.299 + (double)src.getPixel(i,j,GREEN)*0.587 + (double)src.getPixel(i,j,BLUE)*0.114;
// 			//avg /= 3;

// 			++hist[checkValue(avg)];
// 		}
// 	}
// }

//used for debugging and to check if two images are equal.
// void utilities::checkEquality(image& img1, image& img2) {

// 	if(img1.getNumberOfRows() != img2.getNumberOfRows()) {
// 		cout << "\nRows Unequal. Images are not the same." << endl;
// 		return;
// 	}

// 	if(img1.getNumberOfColumns() != img2.getNumberOfColumns()) {
// 		cout << "\nColumns Unequal. Images are not the same." << endl;
// 		return;
// 	}

// 	cout << "\nStarting equal for loop" << endl;

// 	for(int i = 0; i < img1.getNumberOfRows(); ++i)
// 		for(int j = 0; j < img1.getNumberOfColumns(); ++j) {

// 			if(img1.getPixel(i,j,RED) != img2.getPixel(i,j,RED)) {
// 				cout << "Pixel RED " << img1.getPixel(i,j,RED) << " " << img2.getPixel(i,j,RED) << " " << i << ":" << j << endl;
// 			}
// 			// if(img1.getPixel(i,j,GREEN) != img2.getPixel(i,j,GREEN))
// 			// 	cout << "\nPixel GREEN " << img1.getPixel(i,j,GREEN) << " " << img2.getPixel(i,j,GREEN) << " unequal" << endl;
// 			// if(img1.getPixel(i,j,BLUE) != img2.getPixel(i,j,BLUE))
// 			// 	cout << "\nPixel BLUE " << img1.getPixel(i,j,BLUE) << " " << img2.getPixel(i,j,BLUE) << " unequal" << endl;
// 		}

// 	//cout << "\nImages are equal" << endl;
// }

/*----------------------------------------------------------------------------*/
//OLD SHIT
/*----------------------------------------------------------------------------*/
// void utilities::roiSmooth1D(image& src, image& tgt, int ws, pair<int, int> startPoints, pair<int, int> size) {

// 	int m = (ws-1)/2, avgr, avgg, avgb;
// 	image intermediate;
// 	pair<int, int> endPoints;

// 	//copies image and calculates endpoints.
// 	intermediate.copyImage(src);
// 	endPoints.first = startPoints.first + size.first;
// 	endPoints.second = startPoints.second + size.second;

// 	//Horizontal 1D
// 	for(int i = startPoints.first + m; i < endPoints.first - m; ++i) {

// 		avgr = 0, avgg = 0, avgb = 0;
// 		for(int j = startPoints.second + m; j < endPoints.second - m; ++j) {

// 			if(j == startPoints.second + m) {

// 				for(int k = j-m; k <= j+m; ++k) {
// 					avgr += src.getPixel(i,k,RED);
// 					avgg += src.getPixel(i,k,GREEN);
// 					avgb += src.getPixel(i,k,BLUE);
// 				}

// 				intermediate.setPixel(i,j,RED,avgr/ws);
// 				intermediate.setPixel(i,j,GREEN,avgg/ws);
// 				intermediate.setPixel(i,j,BLUE,avgb/ws);
// 			}

// 			else {

// 				avgr = avgr - src.getPixel(i,j-1-m,RED) + src.getPixel(i,j+m,RED);
// 				avgg = avgg - src.getPixel(i,j-1-m,GREEN) + src.getPixel(i,j+m,GREEN);
// 				avgb = avgb - src.getPixel(i,j-1-m,BLUE) + src.getPixel(i,j+m,BLUE);


// 				intermediate.setPixel(i,j,RED,avgr/ws);
// 				intermediate.setPixel(i,j,GREEN,avgg/ws);
// 				intermediate.setPixel(i,j,BLUE,avgb/ws);
// 			}

// 		}
// 	}


// 	//Vertical 1D
// 	for(int j = startPoints.second + m; j < endPoints.second - m; ++j) {

// 		avgr = 0, avgg = 0, avgb = 0;
// 		for(int i = startPoints.first + m; i < endPoints.first - m; ++i) {


// 			if(i == startPoints.first + m) {

// 				for(int k = i-m; k <= i+m; ++k) {

// 					avgr += intermediate.getPixel(k,j,RED);
// 					avgg += intermediate.getPixel(k,j,GREEN);
// 					avgb += intermediate.getPixel(k,j,BLUE);

// 				}

// 				tgt.setPixel(i,j,RED,avgr/ws);
// 				tgt.setPixel(i,j,GREEN,avgg/ws);
// 				tgt.setPixel(i,j,BLUE,avgb/ws);
// 			}

// 			else {

// 				avgr = avgr - intermediate.getPixel(i-1-m, j,RED) + intermediate.getPixel(i+m,j,RED);
// 				avgg = avgg - intermediate.getPixel(i-1-m, j,GREEN) + intermediate.getPixel(i+m,j,GREEN);
// 				avgb = avgb - intermediate.getPixel(i-1-m, j,BLUE) + intermediate.getPixel(i+m,j,BLUE);

// 				tgt.setPixel(i,j,RED,avgr/ws);
// 				tgt.setPixel(i,j,GREEN,avgg/ws);
// 				tgt.setPixel(i,j,BLUE,avgb/ws);
// 			}
// 		}
// 	}
// }


// //
// void utilities::roiSmooth2D(image& src, image& tgt, int ws, pair<int, int> startPoints, pair<int, int> size) {

// 	pair<int, int> endPoints;
// 	int m = (ws-1)/2;

// 	int avgr, avgg, avgb;

// 	endPoints.first = startPoints.first + size.first;
// 	endPoints.second = startPoints.second + size.second;

// 	for(int i = startPoints.first + m; i < endPoints.first - m; ++i) {
		
// 		for(int j = startPoints.second + m; j < endPoints.second - m; ++j) {

// 			//avg = 0;
// 			avgr = 0, avgg = 0, avgb = 0;
// 			for(int k = i-m; k <= i+m; ++k) {
// 				for(int l = j-m; l <= j+m; ++l) {
// 					//avg += src.getPixel(k,l);
// 					avgr += src.getPixel(k,l,RED);
// 					avgg += src.getPixel(k,l,GREEN);
// 					avgb += src.getPixel(k,l,BLUE);
// 				}
// 			}
			
// 			//
// 			//avg /= (ws*ws);
// 			//tgt.setPixel(i,j,avg);
// 			tgt.setPixel(i,j,RED,avgr/(ws*ws));
// 			tgt.setPixel(i,j,GREEN,avgg/(ws*ws));
// 			tgt.setPixel(i,j,BLUE,avgb/(ws*ws));
// 		}
// 	}
// }



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