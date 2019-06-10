#include "image.h"
#include "utilities.h"
#include <strings.h>
#include <string.h>

//MY INCLUDES
#include <fstream>
#include "hsi.h"
#include "opencv.h"

using namespace std;

#define MAXLEN 256
#define MAXROI 3

int main (int argc, char** argv)
{
	image src, tgt;
	string infile, outfile, pch, strtemp;
	ifstream fp;
	int count;
	hsi interm;
	
	

	//attempts to open parameters file.
	fp.open(argv[1],fstream::in);
	if(!fp.is_open()) {
		cout << "\nCant open parameters file: " << argv[1] << endl;
		return -1;
	}

	//loop that read every line in parameters file.
	while(fp) {
		
		//checks if commented out.
		//cout << "\npeek: " << (char)fp.peek() << endl;
		if(fp.peek() == '#') {
			getline(fp,strtemp);
			//cout << "\nstrtemp: " << strtemp << endl;
			if(fp.eof())
				break;
			continue;
		}

		//gets src image filename.
		fp >> infile;
		
		//reads output filename.
		fp >> outfile;

		//reads number of processes on image, max is 3.
		fp >> count;
		if(count > MAXROI) {
			cout << "\nMax Number of ROIs/Processes is " << MAXROI << ". Cannot Process Image: " << infile << endl;
			getline(fp,strtemp);
			continue;
		}

		//reads then copies image and runs loop for up to 3 processes.
		if(!src.read(infile)) {
			getline(fp,strtemp);
			continue;
		}

		//copies source to target and sets filename.
		tgt = src;
		tgt = outfile;

		//copies and converts source to HSI and sets filename.
		interm = src;
		interm = outfile;

		//opencv objects
		opencv ocv(infile);
		bool savetgt = true;
		
		if(fp.eof()) break;
		for(int i = 0; i < count; ++i) {
			
			//reads process name.
			pch = " ";
			fp >> pch;

			clock_t startTime = clock();
			//grayscale binarization/threshold process.
			if(!strncasecmp(pch.c_str(),"bin",MAXLEN)) {
				
				int thresh;
				pair<int, int> start, size;

				fp >> thresh;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				
				utilities::roiBinarize(src, tgt, start, size, thresh);
				
			}

			//color binarization/threshold process.
			else if (!strncasecmp(pch.c_str(),"binc",MAXLEN)) {
				
				//
				int tc, colors[3];
				pair<int,int> start, size;

				//caps data from parameters file.
				fp >> tc;
				fp >> colors[0] >> colors[1] >> colors[2];
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				//
				utilities::roiBinarizeColor(src, tgt, tc, colors, start, size);
			}

			//1D smoothing process.
			else if(!strncasecmp(pch.c_str(),"1Dsmooth",MAXLEN)) {

				//	
				pair<int, int> start, size;
				int ws;

				//pulling data needed out of parameters file.
				fp >> ws;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				//makes sure ws > 3 and odd.
				if(ws % 2 == 0 || ws < 3) {

					cout << "\nWindow size must be an odd number 3 or greater for smothing. 1DSmooth will not be processed on " << infile << endl;
					continue;
				}
				
				utilities::roiSmooth1DAdaptive(src, tgt, ws, start, size);
			}

			//2D smoothing process.
			else if(!strncasecmp(pch.c_str(),"2Dsmooth",MAXLEN)) {

				pair<int, int> start, size;
				int ws;

				//collects data from parameters file.
				fp >> ws;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				//makes sure ws > 3 and odd.
				if(ws % 2 == 0 || ws < 3) {

					cout << "\nWindow size must be an odd number 3 or greater for smothing. 2DSmooth will not be processed on " << infile << endl;
					continue;
				}
				
				utilities::roiSmooth2DAdaptive(src, tgt, ws, start, size);
			}


			//hist stretch grayscale.
			else if(!strncasecmp(pch.c_str(),"sgs",MAXLEN)) {

				//cout << "in stretchgs" << endl;

				//variables needed for process.
				int histbefore[256] = {0};
				int histafter[256] = {0};
				pair<int, int> start, size, ab, cd;
				bool inbounds = true;
				string key("."), histfn(outfile), histtag("_hist-roi-");

				//appends ROI number and period to histogram tag.
				histtag += (char)('1' + i);
				histtag += '.';

				//captures ROI data from parameters file.
				fp >> ab.first >> ab.second;
				fp >> cd.first >> cd.second;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				//checks if ROI is inbounds.
				inbounds = utilities::stretchInBounds(src,infile,start,size,ab,cd);
				if(!inbounds)
					continue;

				//adds tag to outfile and saves it to ofname.
				size_t pos = histfn.rfind(key);
				if(pos != std::string::npos)
					histfn.replace(pos,key.size(),histtag);

				//creates hist from src, stretches it to tgt, creates hist from tgt.
				utilities::histCreate(src, histbefore, start, size);
				utilities::histStretchGS(src,tgt,ab,cd,start,size);
				utilities::histCreate(tgt,histafter,start,size);
				utilities::histSave(histbefore,histafter,histfn);
			}

			//hist stretch color.
			else if(!strncasecmp(pch.c_str(),"sca",MAXLEN)) {

				int histr1[256] = {0}, histg1[256] = {0}, histb1[256] = {0}, histr2[256] = {0}, histg2[256] = {0}, histb2[256] = {0};
				//int histall1[256] = {0}, histall2[256] = {0};
				pair<int, int> start, size, ab, cd;
				bool inbounds = true;
				size_t pos;
				string key("."), histfnr(outfile), histfng(outfile), histfnb(outfile), histtagr("_hist-roi-"), histtagg("_hist-roi-"), histtagb("_hist-roi-");
				//string histfnall(outfile), histtagall("_hist-roi-");

				histtagr += (char)('1' + i);
				histtagg += (char)('1' + i);
				histtagb += (char)('1' + i);
				//histtagall += (char)('1' + i);

				histtagr += "-red.";
				histtagg += "-green.";
				histtagb += "-blue.";
				//histtagall += ".";

				pos = histfnr.rfind(key);
				if(pos != std::string::npos)
					histfnr.replace(pos,key.size(),histtagr);
				
				pos = histfng.rfind(key);
				if(pos != std::string::npos)
					histfng.replace(pos,key.size(),histtagg);
				
				pos = histfnb.rfind(key);
				if(pos != std::string::npos)
					histfnb.replace(pos,key.size(),histtagb);
				
				// pos = histfnall.rfind(key);
				// if(pos != std::string::npos)
				// 	histfnall.replace(pos,key.size(),histtagall);

				//captures ROI data from parameters file.
				fp >> ab.first >> ab.second;
				fp >> cd.first >> cd.second;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				//checks if ROI is inbounds.
				inbounds = utilities::stretchInBounds(src,infile,start,size,ab,cd);
				if(!inbounds)
					continue;

				//all colors histogram
				//utilities::histCreateRGB(src,histall1,start,size);
				
				//creates histogram for each individual color RGB.
				utilities::histCreateRGB(src,histr1,histg1,histb1,start,size);
				
				//stretches all colors RGB.
				utilities::histStretchRGB(src,tgt,ab,cd,start,size);

				//all colors after histogram.
				//utilities::histCreateRGB(src,histall2,start,size);

				//creats after stretch histograms for each individual color RGB.
				utilities::histCreateRGB(tgt,histr2,histg2,histb2,start,size);
				
				//utilities::histSave(histall1,histall2,histfnall);
				utilities::histSave(histr1,histr2,histfnr);
				utilities::histSave(histg1,histg2,histfng);
				utilities::histSave(histb1,histb2,histfnb);
			}

			else if(!strncasecmp(pch.c_str(),"scr",MAXLEN)) {

				int hist1[256] = {0}, hist2[256] = {0};
				pair<int,int> start, size, ab, cd;
				bool inbounds = true;
				size_t pos;
				string key("."), histfn(outfile), histtag("_hist-roi-");

				histtag += (char)('1' + i);
				histtag += "-red.";

				pos = histfn.rfind(key);
				if(pos != std::string::npos)
					histfn.replace(pos,key.size(),histtag);

				//captures ROI data from parameters file.
				fp >> ab.first >> ab.second;
				fp >> cd.first >> cd.second;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				//checks if ROI is inbounds.
				inbounds = utilities::stretchInBounds(src,infile,start,size,ab,cd);
				if(!inbounds)
					continue;

				utilities::histCreateRGB(src,hist1,start,size,RED);
				utilities::histStretchRGB(src,tgt,ab,cd,start,size,RED);
				utilities::histCreateRGB(tgt,hist2,start,size,RED);
				utilities::histSave(hist1,hist2,histfn);

			}

			else if(!strncasecmp(pch.c_str(),"scg",MAXLEN)) {

				int hist1[256] = {0}, hist2[256] = {0};
				pair<int,int> start, size, ab, cd;
				bool inbounds = true;
				size_t pos;
				string key("."), histfn(outfile), histtag("_hist-roi-");

				histtag += (char)('1' + i);
				histtag += "-green.";

				pos = histfn.rfind(key);
				if(pos != std::string::npos)
					histfn.replace(pos,key.size(),histtag);

				//captures ROI data from parameters file.
				fp >> ab.first >> ab.second;
				fp >> cd.first >> cd.second;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				//checks if ROI is inbounds.
				inbounds = utilities::stretchInBounds(src,infile,start,size,ab,cd);
				if(!inbounds)
					continue;

				utilities::histCreateRGB(src,hist1,start,size,GREEN);
				utilities::histStretchRGB(src,tgt,ab,cd,start,size,GREEN);
				utilities::histCreateRGB(tgt,hist2,start,size,GREEN);
				utilities::histSave(hist1,hist2,histfn);

			}

			else if(!strncasecmp(pch.c_str(),"scb",MAXLEN)) {

				int hist1[256] = {0}, hist2[256] = {0};
				pair<int,int> start, size, ab, cd;
				bool inbounds = true;
				size_t pos;
				string key("."), histfn(outfile), histtag("_hist-roi-");

				histtag += (char)('1' + i);
				histtag += "-blue.";

				pos = histfn.rfind(key);
				if(pos != std::string::npos)
					histfn.replace(pos,key.size(),histtag);

				//captures ROI data from parameters file.
				fp >> ab.first >> ab.second;
				fp >> cd.first >> cd.second;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				//checks if ROI is inbounds.
				inbounds = utilities::stretchInBounds(src,infile,start,size,ab,cd);
				if(!inbounds)
					continue;

				utilities::histCreateRGB(src,hist1,start,size,BLUE);
				utilities::histStretchRGB(src,tgt,ab,cd,start,size,BLUE);
				utilities::histCreateRGB(tgt,hist2,start,size,BLUE);
				utilities::histSave(hist1,hist2,histfn);

			}

			else if(!strncasecmp(pch.c_str(),"si",MAXLEN)) {

				int hist1[256] = {0}, hist2[256] = {0};
				pair<int,int> start, size, ab, cd;
				bool inbounds = true;
				size_t pos;
				string key("."), histfn(outfile), histtag("_hist-roi-");

				histtag += (char)('1' + i);
				histtag += "-I.";

				pos = histfn.rfind(key);
				if(pos != std::string::npos)
					histfn.replace(pos,key.size(),histtag);

				//captures ROI data from parameters file.
				fp >> ab.first >> ab.second;
				fp >> cd.first >> cd.second;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				inbounds = utilities::stretchInBounds(src,infile,start,size,ab,cd);
				if(!inbounds)
					continue;

				interm.histI(hist1);
				interm.stretchI(ab,cd,start,size);
				interm.histI(hist2);
				tgt.hsiROI(interm,start,size);
				utilities::histSave(hist1,hist2,histfn);

			}

			else if(!strncasecmp(pch.c_str(),"sh",MAXLEN)) {
	
				int hist1[256] = {0}, hist2[256] = {0};
				pair<int,int> start, size, ab, cd;
				size_t pos;
				string key("."), histfn(outfile), histtag("_hist-roi-");

				histtag += (char)('1' + i);
				histtag += "-H.";

				pos = histfn.rfind(key);
				if(pos != std::string::npos)
					histfn.replace(pos,key.size(),histtag);

				//captures ROI data from parameters file.
				fp >> ab.first >> ab.second;
				fp >> cd.first >> cd.second;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				

				interm.histH(hist1);
				interm.stretchH(ab,cd,start,size);
				interm.histH(hist2);
				tgt.hsiROI(interm,start,size);
				utilities::histSave(hist1,hist2,histfn);
			}

			else if(!strncasecmp(pch.c_str(),"ss",MAXLEN)) {

				int hist1[256] = {0}, hist2[256] = {0};
				pair<int,int> start, size, ab, cd;
				bool inbounds = true;
				size_t pos;
				string key("."), histfn(outfile), histtag("_hist-roi-");

				histtag += (char)('1' + i);
				histtag += "-S.";

				pos = histfn.rfind(key);
				if(pos != std::string::npos)
					histfn.replace(pos,key.size(),histtag);

				//captures ROI data from parameters file.
				fp >> ab.first >> ab.second;
				fp >> cd.first >> cd.second;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				inbounds = utilities::stretchInBounds(src,infile,start,size,ab,cd);
				if(!inbounds)
					continue;

				interm.histS(hist1);
				interm.stretchS(ab,cd,start,size);
				interm.histS(hist2);
				tgt.hsiROI(interm,start,size);
				utilities::histSave(hist1,hist2,histfn);
			}

			else if(!strncasecmp(pch.c_str(),"edgs",MAXLEN)) {

				int thresh, degree;
				pair<int,int> start, size;
				vector< vector<edgeData> > edges;

				fp >> thresh >> degree;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				src.prewitt3gs();
				tgt = src.getEdges();

				// image direction;
				// tgt.saveDirection(direction);
				// direction.save(outfile + "-direction.pgm");
				
				tgt.binGradient(thresh,degree);
				tgt.saveGradient(start,size);
				
				
			}

			else if(!strncasecmp(pch.c_str(),"edc",MAXLEN)) {

				int thresh, degree, color;
				pair<int,int> start, size;
				vector< vector<edgeData> > edges;

				fp >> thresh >> degree >> color;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				src.prewitt3(color);
				tgt = src.getEdges();

				// image direction;
				// tgt.saveDirection(direction);
				// direction.save(outfile + "-direction.pgm");
				
				tgt.binGradient(thresh,degree);
				tgt.saveGradient(start,size);
			}

			else if(!strncasecmp(pch.c_str(),"edrgb",MAXLEN)) {

				int thresh, degree;
				pair<int,int> start, size;
				//vector< vector<edgeData> > edges;

				fp >> thresh >> degree;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				src.prewitt3rgb();
				//tgt = src.getEdges();
				tgt.setEdges(RED,src.getEdges(RED));
				tgt.setEdges(GREEN,src.getEdges(GREEN));
				tgt.setEdges(BLUE,src.getEdges(BLUE));

				// image direction;
				// tgt.saveDirection(direction);
				// direction.save(outfile + "-direction.pgm");
				
				tgt.binGradientRGB(thresh,degree);
				tgt.saveGradient(start,size);
			}

			else if(!strncasecmp(pch.c_str(),"edi",MAXLEN)) {

				int thresh, degree;
				pair<int,int> start, size;
				vector< vector<edgeData> > edges;

				fp >> thresh >> degree;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				interm = src;

				interm.prewitt3();
				//interm.saveGradientI(start,size);
				//interm.stretchI(make_pair(0,thresh),make_pair(0,255),make_pair(0,0),make_pair(src.getNumberOfRows(),src.getNumberOfColumns()));
				//interm.saveEdgesI();
				//interm.binGradientI(thresh,degree);
				
				
				
				tgt.setEdges(RED, interm.getEdges());
				tgt.binGradient(thresh,degree);
				tgt.saveGradientRed(start,size);
			}

			else if(!strncasecmp(pch.c_str(),"edh",MAXLEN)) {

				int thresh, degree;
				pair<int,int> start, size;
				vector< vector<edgeData> > edges;

				fp >> thresh >> degree;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				interm = src;

				interm.prewitt3();
				//interm.saveGradientI(start,size);
				//interm.stretchI(make_pair(0,thresh),make_pair(0,255),make_pair(0,0),make_pair(src.getNumberOfRows(),src.getNumberOfColumns()));
				//interm.saveEdgesI();
				//interm.binGradientI(thresh,degree);
				
				
				
				tgt.setEdges(RED, interm.getEdges(0));
				tgt.binGradient(thresh,degree);
				tgt.saveGradientRed(start,size);

			}

			else if(!strncasecmp(pch.c_str(),"eds",MAXLEN)) {

				int thresh, degree;
				pair<int,int> start, size;
				vector< vector<edgeData> > edges;

				fp >> thresh >> degree;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				interm = src;

				interm.prewitt3();
				//interm.saveGradientI(start,size);
				//interm.stretchI(make_pair(0,thresh),make_pair(0,255),make_pair(0,0),make_pair(src.getNumberOfRows(),src.getNumberOfColumns()));
				//interm.saveEdgesI();
				//interm.binGradientI(thresh,degree);
				
				
				
				tgt.setEdges(RED, interm.getEdges(1));
				tgt.binGradient(thresh,degree);
				tgt.saveGradientRed(start,size);

			}


			else if(!strncasecmp(pch.c_str(),"edgs5",MAXLEN)) {

				int thresh, degree;
				pair<int,int> start, size;
				vector< vector<edgeData> > edges;

				fp >> thresh >> degree;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				src.prewitt5gs();
				tgt = src.getEdges();

				// image direction;
				// tgt.saveDirection(direction);
				// direction.save(outfile + "-direction.pgm");
				
				tgt.binGradient(thresh,degree);
				tgt.saveGradient(start,size);
				
				
			}

			else if(!strncasecmp(pch.c_str(),"sobelgs",MAXLEN)) {

				int thresh;
				pair<int,int> start, size;
				
				fp >> thresh;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				ocv.sobelgs(thresh, start, size);

				savetgt = false;
			}

			else if(!strncasecmp(pch.c_str(),"sobelhsv",MAXLEN)) {

				int thresh;
				pair<int,int> start, size;
				
				fp >> thresh;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				ocv.sobelhsv(thresh, start, size);

				savetgt = false;
			}

			else if(!strncasecmp(pch.c_str(),"histcv",MAXLEN)) {

				pair<int,int> start, size;
				
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				ocv.histEqual(start, size);

				savetgt = false;
			}

			else if(!strncasecmp(pch.c_str(),"histcvrgb",MAXLEN)) {

				pair<int,int> start, size;
				
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				ocv.histEqualRGB(start, size);

				savetgt = false;
			}

			else if(!strncasecmp(pch.c_str(),"cannygs",MAXLEN)) {

				pair<int,int> start, size;
				int thresh;//, ratio;
				
				fp >> thresh;// >> ratio;
				fp >> start.first >> start.second;
				fp >> size.first >> size.second;

				ocv.cannygs(thresh, start, size);

				savetgt = false;
			}

			//
			else if(!strncasecmp(pch.c_str(),"plus",MAXLEN)) {

				image temp;
				
				fp >> pch;
				temp.read(pch);
				tgt += temp;
			}

			//
			else {
				cout << "\nNo Function: " << pch << endl;
				break;
			}

			cout << pch << " : " << (double)(clock()-startTime)/(double)CLOCKS_PER_SEC*1000 << " ms" << endl;
		}

		if(savetgt)
			tgt.save();
		else
			ocv.save(outfile);


		getline(fp,strtemp);
		

	}

	fp.close();
	return 0;
}
