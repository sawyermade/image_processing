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

class hsi;

class image 
{
private:
   imageData data;
   int getint(FILE *fp);
   string fname;
   
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
};

#endif

