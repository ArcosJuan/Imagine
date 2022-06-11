#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include "helper.h"
#define ONE_OVER_BILLION 1E-9


using namespace std;

int main(int argc , char* argv[]){

	const vector<string> args(argv, argv + argc);
	vector<vector<string>> filters = get_filters(args);
	string _help = help(args);
	if (!_help.empty()) cout << _help;

	if (filters.size() > 0){

		ppm img(argv[1]);
		string out = string(argv[2]);
		bool _single_t = single_thread(args);

		cout << "Applying filters:"<< endl;
		struct timespec start, stop;    	
		clock_gettime(CLOCK_REALTIME, &start);

	    for (vector<string> parameters: filters) {
	    	string filter = string(parameters[0]);
	    	
	    	unsigned int n_threads;
	    	if (_single_t) n_threads = 1; 
	    	else n_threads = stoi(parameters[1]);
	    	
	    	string arg_1;
	    	string arg_2;
	    	if (parameters.size() >= 3 - _single_t) arg_1 = parameters[2 - _single_t];
	    	if (parameters.size() >= 4 - _single_t) arg_2 = parameters[3 - _single_t];

	    	cout << filter << endl;

			if (filter == "plain") plain(img, (unsigned char)stoi(arg_1));
			else if (filter == "blackwhite") blackWhite(img);
			else if (filter == "shades") shades(img, (unsigned char)stoi(arg_1));
			else if (filter == "contrast") contrast(img, stof(arg_1));
			else if (filter == "bright") brightness(img, stof(arg_1));
			else if (filter == "frame") frame(img, stoi(arg_1), (unsigned char)stoi(arg_2));
			else if (filter == "zoom") zoom(img, stoi(arg_1));
			else if (filter == "blur") boxBlur(img, stoi(arg_1));
			else if (filter == "edge") edgeDetection(img);	
			else if (filter == "merge") {
				ppm img2(arg_1);
				merge(img, img2, stof(arg_2));
			}
	    } 

	   	clock_gettime(CLOCK_REALTIME, &stop);

		double accum;
		accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
		printf("%lf s\n", accum);

		cout << "Writing image. . ." << endl;
		img.write(out);	
		    
		cout << "DONE!" << endl;
	}

	return 0;
}