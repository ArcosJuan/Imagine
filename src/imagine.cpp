#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include <filesystem>
#include "helper.h"
#define ONE_OVER_BILLION 1E-9


using namespace std;
using namespace filesystem;

double apply_filters(ppm& img, string output, vector<vector<string>> filters, int n_threads, bool filter_thread){

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

    

		if (filter == "plain") plain(img, (unsigned char)stoi(arg_1), nf_threads);
		else if (filter == "blackwhite") blackWhite(img, nf_threads);
		else if (filter == "shades") shades(img, (unsigned char)stoi(arg_1), nf_threads);
		else if (filter == "contrast") contrast(img, stof(arg_1), nf_threads);
		else if (filter == "bright") brightness(img, stof(arg_1), nf_threads);
		else if (filter == "frame") frame(img, stoi(arg_1), (unsigned char)stoi(arg_2), nf_threads);
		else if (filter == "zoom") zoom(img, stoi(arg_1), nf_threads);
		else if (filter == "blur") boxBlur(img, stoi(arg_1), nf_threads);
		else if (filter == "edge") edgeDetection(img, nf_threads);	
		else if (filter == "merge") {
			ppm img2(arg_1);
			merge(img, img2, stof(arg_2), nf_threads);
		}
    } 

    clock_gettime(CLOCK_REALTIME, &stop);

	double accum;
	accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	printf("  Image filtering took: %lf s\n", accum);

	cout << "  Writing image. . .";
	img.write(output);	
	cout << " DONE!" << endl << endl;

	return accum;
}


int main(int argc , char* argv[]){

	const vector<string> args(argv, argv + argc);
	vector<vector<string>> filters = get_filters(args);
	string _help = help(args);
	if (!_help.empty()) cout << _help;

	if (filters.size() > 0){

		bool is_batch = batch(args);
		string input = string(argv[1]);
		string output = string(argv[2]);
		unsigned int n_threads;

		// Defines whether to define a number of threads per filter.
		bool filter_thread = (single_thread(args) | multi_thread(args));
		if (single_thread(args)) n_threads = 1;
		else if (multi_thread(args)) n_threads = get_multi_thread(args);
		else n_threads = 0;


		cout << "Applying filters:"<< endl;
		for (vector<string> filter: filters) cout << "  - " << filter[0] << endl;
		cout << endl;

		if (!is_batch) {
			path img_path = path(input);
			if (img_path.extension() != ".ppm") {
				cout << "The argument passed is not an image!" << endl;
				cout << help("usage"); return 1;
			}

			cout << "On image: " << string(img_path.stem()) << endl;
			ppm img(input);
			apply_filters(img, output, filters, n_threads, filter_thread);
		}
		else{
			double total_time = 0;

			path out_dir = path(output); 
			// Create the directory if it doesn't exist.
			if (!is_directory(out_dir)) create_directory(out_dir);

	 		for (const auto & image : directory_iterator(input)){
				auto img_path = image.path();
				if (img_path.extension() != ".ppm") continue;
				cout << "On image: " << string(img_path.stem()) << endl;
				ppm img(img_path);

				path out_path = out_dir; 
				out_path /= string(img_path.stem()) + ".ppm";

				total_time += apply_filters(img, out_path, filters, n_threads, filter_thread);
			}

			printf("Total Time: %lf s\n", total_time);
		}
	}

	return 0;
}