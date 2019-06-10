#include "image.h"
#include "utilities.h"
#include <strings.h>
#include <string.h>

using namespace std;

#define MAXLEN 256
#define MAXROI 3

int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char outfile[MAXLEN], infile[MAXLEN];
	char *pch;
	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	int count;
	while(fgets(str,MAXLEN,fp) != NULL) {
		
		//reads input filename.
		pch = strtok(str, " ");
		if(!strncasecmp(pch, "#", MAXLEN))
			continue;
		strcpy(infile, pch);
		src.read(pch);

		//reads output filename.
		pch = strtok(NULL, " ");
		strcpy(outfile, pch);

		//reads number of processes on image, max is 3.
		count = atoi(pch = strtok(NULL, " "));
		if(count > MAXROI){
			cout << "\nMax Number of ROIs/Functions is " << MAXROI << ". Cannot Process Image: " << infile << endl;
			continue;
		}

		//copies image and runs loop for up to 3 processes.
		tgt.copyImage(src);
		for(int i = 0; i < count; ++i) {
			
			//reads function name.
			pch = strtok(NULL, " ");
			//cout << "\nFunction " << pch << endl;

			//grayscale binarization/threshold process.
			if(!strncasecmp(pch, "bin", MAXLEN)) {
				
				pair<int, int> startPoints, size;

				int thresh = atoi(pch = strtok(NULL, " "));
				startPoints.first = atoi(pch = strtok(NULL, " "));
				startPoints.second = atoi(pch = strtok(NULL, " "));
				size.first = atoi(pch = strtok(NULL, " "));
				size.second = atoi(pch = strtok(NULL, " "));

				utilities::roiThresh(src, tgt, startPoints, size, thresh);
			}

			//color binarization/threshold process.
			else if (!strncasecmp(pch,"binc",MAXLEN)) {
				
				//
				int tc = atoi(pch = strtok(NULL, " "));
				int colors[3] = {atoi(pch = strtok(NULL, " ")), atoi(pch = strtok(NULL, " ")), atoi(pch = strtok(NULL, " "))};
				pair<int, int> startPoints, size;

				//
				startPoints.first = atoi(pch = strtok(NULL, " "));
				startPoints.second = atoi(pch = strtok(NULL, " "));
				size.first = atoi(pch = strtok(NULL, " "));
				size.second = atoi(pch = strtok(NULL, " "));

				//
				utilities::roiBinarizeColor(src, tgt, tc, colors, startPoints, size);
			}

			//1D smoothing process.
			else if(!strncasecmp(pch,"1Dsmooth",MAXLEN)) {

				//	
				pair<int, int> startPoints, size;
				int ws;

				ws = atoi(pch = strtok(NULL, " "));
				startPoints.first = atoi(pch = strtok(NULL, " "));
				startPoints.second = atoi(pch = strtok(NULL, " "));
				size.first = atoi(pch = strtok(NULL, " "));
				size.second = atoi(pch = strtok(NULL, " "));
					
				utilities::roiSmooth1D(src, tgt, ws, startPoints, size);
			}

			//2D smoothing process.
			else if(!strncasecmp(pch,"2Dsmooth",MAXLEN)) {

				//	
				pair<int, int> startPoints, size;
				int ws;

				ws = atoi(pch = strtok(NULL, " "));
				startPoints.first = atoi(pch = strtok(NULL, " "));
				startPoints.second = atoi(pch = strtok(NULL, " "));
				size.first = atoi(pch = strtok(NULL, " "));
				size.second = atoi(pch = strtok(NULL, " "));
					
				utilities::roiSmooth2D(src, tgt, ws, startPoints, size);
			}

			//
			else {
				cout << "\nNo Function: " << pch << endl;
				break;
			}
		}

		tgt.save(outfile);

	}

	fclose(fp);
	return 0;
}

