#ifndef UTILITIES_H
#define UTILITIES_H

#include "image.h"
#include <sstream>
#include <math.h>

class utilities
{
	public:
		utilities();
		virtual ~utilities();
		static std::string intToString(int number);
		static int checkValue(int value);

		//MY SHIT
		static void roiThresh(image &src, image &tgt, pair<int, int>startPoints, pair<int, int> sizexy, int thresh);
		static void roiBinarizeColor(image &src, image &tgt, int threshold, int colors[], pair<int, int> startPoints, pair<int, int> sizexy);
		static void roiSmooth1D(image& src, image& tgt, int ws, pair<int, int> startPoints, pair<int, int> size);
		static void roiSmooth2D(image& src, image& tgt, int ws, pair<int, int> startPoints, pair<int, int> size);
};

#endif
