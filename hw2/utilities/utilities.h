#ifndef UTILITIES_H
#define UTILITIES_H

#define HISTMAX 256

#include "image.h"
#include <sstream>
#include <math.h>
//#include <string>

class utilities
{
	public:
		utilities();
		virtual ~utilities();
		static std::string intToString(int number);
		static int checkValue(int value);

		/***MY SHIT***/
		//pj1
		static void roiBinarize(image &src, image &tgt, pair<int, int>startPoints, pair<int, int> sizexy, int thresh);
		static void roiBinarizeColor(image &src, image &tgt, int threshold, int colors[], pair<int, int> startPoints, pair<int, int> sizexy);
		static void roiSmooth1DAdaptive(image& src, image& tgt, int ws, pair<int, int> start, pair<int, int> size);
		static void roiSmooth2DAdaptive(image& src, image& tgt, int ws, pair<int, int> startPoints, pair<int, int> size);
		//static void checkEquality(image& img1, image& img2);
		//static void roiSmooth1D(image& src, image& tgt, int ws, pair<int, int> startPoints, pair<int, int> size);
		//static void roiSmooth2D(image& src, image& tgt, int ws, pair<int, int> startPoints, pair<int, int> size);

		//pj2
		static void histCreate(image& src, int hist[256], pair<int, int> start , pair<int, int> stop);
		static void histPrint(int hist[256]); //debug
		static void histSave(int hist[256], string& fname);
		static void histStretchGS(image& src,image& tgt,pair<int,int> ab,pair<int,int> cd,pair<int,int> start,pair<int,int> stop);
		static void stringToChar(string&,char*);
		static void histSave(int hbefore[256],int hafter[256],string& fname);
		static bool stretchInBounds(image& src, string& infile, pair<int,int> start, pair<int,int> size, pair<int,int> ab, pair<int,int> cd);
		static void histCreateRGB(image& src, int hist[256], pair<int, int> start , pair<int, int> size, const int COLOR);
		static void histStretchRGB(image& src, image& tgt, pair<int,int> ab, pair<int,int> cd, pair<int,int> start, pair<int,int> size, const int COLOR);
		static void histStretchRGB(image& src, image& tgt, pair<int,int> ab, pair<int,int> cd, pair<int,int> start, pair<int,int> size);
		static void histCreateRGB(image& src, int histr[256], int histg[256], int histb[256], pair<int,int> start, pair<int,int> size);
		//static void histCreateRGB(image& src, int hist[256], pair<int, int> start , pair<int, int> size);
};

#endif
