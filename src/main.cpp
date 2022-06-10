#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#include <unistd.h>
#include <fstream>      // std::ofstream
#define ONE_OVER_BILLION 1E-9


using namespace std;

int main(int argc , char* argv[]){

	if(string(argv[1]) == "--help" || string(argv[1]) == "-h"){
		cout << "Usage: ./main <filter> <nthreads> <[p1]> <img1> <custom_output> <[p2]> <img2>" << endl;
		return 0; 
	}
	
	string filter = string(argv[1]);
	unsigned int n = atoi(argv[2]);
	float p1 = atof(argv[3]);
	string img1(argv[4]);
	string out = string(argv[5]);
	
	ppm img(img1);
	
	cout << "Applying filters:"<< endl;
	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);

	if (filter == "plain") plain(img, (unsigned char)p1);
	else if (filter == "blackwhite") blackWhite(img);
	else if (filter == "shades") shades_mt(img, (unsigned char)p1, n);
	else if (filter == "contrast") contrast(img, (float)p1);
	else if (filter == "bright") brightness(img, (float)p1);
	else if (filter == "frame") frame(img, (int)p1);
	else if (filter == "zoom") zoom(img, (int)p1);
	else if (filter == "merge") {
		ppm img2(string{argv[6]});
		merge(img, img2, (float)p1);
	}
	else if (filter == "blur") boxBlur(img, p1);
	else if (filter == "edge") edgeDetection(img);	
   	clock_gettime(CLOCK_REALTIME, &stop);

	double accum;
	accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	printf("%lf s\n", accum);

	cout << "Writing image. . ." << endl;
	img.write(out);	
	    
	cout << "DONE!" << endl;
	return 0;
}