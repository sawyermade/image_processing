#ifndef HSI_H
#define HSI_H

#include <iostream>
#include <cmath>
#include <vector>


#define BLACK 0
#define WHITE 255

#define MAXHUE 360
#define MINHUE 0

using namespace std;

class image;

class hsi {

	private:
		//vars
		int rows, cols;
		string fname;
		vector< vector<double> > hsiH;
		vector< vector<double> > hsiS;
		vector< vector<double> > hsiI;

		//priv funtions.
		void clearVectors();

	public:
		//constructors
		~hsi(void);
		hsi(void);
		hsi(image&);
		hsi(int,int,string&);
		hsi(int,int);
		

		//operators.
		hsi& operator=(image& right);
		hsi& operator=(string& right);


		//functions.
		void convertRGB(image& src);
		void resize(image& src);
		void convertH(image& src);
		void convertS(image& src);
		void convertI(image& src);
		int checkValue(int value);
		void histI(int hist[256]);
		void histS(int hist[256]);
		void histH(int hist[256]);
		


		//get functions.
		double getH(int i, int j);
		double getS(int i, int j); 
		int getI(int i, int j);
		string getFname(void);
		int getRows();
		int getCols();

		//stretching functions.
		void stretchH(pair<int,int> ab, pair<int,int> cd, pair<int,int> start, pair<int,int> size);
		void stretchS(pair<int,int> ab, pair<int,int> cd, pair<int,int> start, pair<int,int> size);
		void stretchI(pair<int,int> ab, pair<int,int> cd, pair<int,int> start, pair<int,int> size);
		void stretchHSI(pair<int,int> ab, pair<int,int> cd, pair<int,int> start, pair<int,int> size);
};

#endif