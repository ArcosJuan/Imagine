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
		unsigned int n_threads;

		// Defines whether to define a number of threads per filter.
		bool filter_thread = (single_thread(args) | multi_thread(args));
		if (single_thread(args)) n_threads = 1;
		else if (multi_thread(args)) n_threads = get_multi_thread(args);
		else n_threads = 0;


		cout << "Applying filters:"<< endl;
		struct timespec start, stop;    	
		clock_gettime(CLOCK_REALTIME, &start);

	    for (vector<string> parameters: filters) {
	    	string filter = string(parameters[0]);
	    	
	    	unsigned int nf_threads;
	    	if(n_threads) nf_threads = n_threads;
    		else n_threads = stoi(parameters[1]);
	    
	    	string arg_1;
	    	string arg_2;
	    	if (parameters.size() >= 3 - filter_thread) arg_1 = parameters[2 - filter_thread];
	    	if (parameters.size() >= 4 - filter_thread) arg_2 = parameters[3 - filter_thread];

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