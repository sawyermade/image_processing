#ifndef IMAGE_H
#define IMAGE_H

// #include "core.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <vector>

using namespace std;

enum channel{RED, GREEN, BLUE, GREY=RED, GRAY=GREY};

struct imageData
{
   vector<int>  redChannel, greenChannel, blueChannel; 
   int numRows, numColumns; 
};

struct edgeData {

	double gradient, direction, dx, dy;
};

class hsi;

class image 
{
private:
   imageData data;
   int getint(FILE *fp);
   string fname;

   //Prewitt 3x3
   vector< vector<edgeData> > edgesr;
   vector< vector<edgeData> > edgesg;
   vector< vector<edgeData> > edgesb;
   
public:
   image();
   image(image &img);
   image(int rows, int columns);
   ~image();
   
   void deleteImage();
   void copyImage(image &img);
   void resize (int numberOfRows, int numberOfColumns);
   void setNumberOfRows(int rows);
   void setNumberOfColumns(int columns);
   void setPixel(const int row, const int col, const int value);
   void setPixel(const int row, const int col, const int rgb, const int value);
   int getPixel(const int row, const int col);
   int getPixel(const int row, const int col, const int rgb);
   int getNumberOfRows();
   int getNumberOfColumns(); 
   vector<int>* getChannel(int rgb);
   
   bool setChannel(int rgb, vector<int> &channel);   
   bool save (char* file);
   bool save (const char* file);
   bool read (char* file);
   bool isInbounds (const int row, const int col);

   //MY SHIT
   image& operator=(image&);
   image& operator+=(image&);
   void setAll(int,int,int);
   bool save(const string&);
   image(int,int,string&);
   image(string&);
   image& operator=(string&);
   bool save(void);
   image& operator=(char*);
   string getFname(void);
   bool read(string& file);
   void hsiROI(hsi& src, pair<int,int> start, pair<int,int> size);
   image& operator=(hsi& src);
   void convertHSI(hsi& src);

   //EDGE DETECTION Prewitt 3x3
   void prewitt3gs();
   void saveGradient(image&);
   double checkValue(double val);
   void saveDirection(image&);
   void saveGradient(pair<int,int>,pair<int,int>);
   image& operator=(vector< vector<edgeData> >);
   void binGradient(int thresh, int degree);
   vector< vector<edgeData> > getEdges();
   void prewitt3(const int COLOR);
   void prewitt3rgb();
   void setEdges(const int COLOR, vector< vector<edgeData> > ogedge);
   vector< vector<edgeData> > getEdges(const int COLOR);
   void binGradientRGB(int thresh, int degree);
   void saveGradientRed(pair<int,int>,pair<int,int>);
   void prewitt5gs();
};

#endif

