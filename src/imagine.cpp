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
		else nf_threads = stoi(parameters[1]);
    
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
		else if (filter == "sharpen") sharpen(img, stoi(arg_1), nf_threads);
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


double simple_load(string input, string output, vector<vector<string>> filters, int n_threads, bool filter_thread){
	path img_path = path(input);
	if (img_path.extension() != ".ppm") {
		cout << "The argument passed is not an image!" << endl;
		cout << help("usage"); return 1;
	}

	cout << "On image: " << string(img_path.stem()) << endl;
	ppm img(input);
	return apply_filters(img, output, filters, n_threads, filter_thread);
}


double batch_load(string input, string output, vector<vector<string>> filters, int n_threads, bool filter_thread, int n_readers){
	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);

	path out_dir = path(output); 
	// Create the directory if it doesn't exist.
	if (!is_directory(out_dir)) create_directory(out_dir);

	// Stores in a vector all the paths of the images.
	vector<path> paths;
	for (const auto & image : directory_iterator(input)){
		auto img_path = image.path();
		if (img_path.extension() != ".ppm") continue;
		paths.push_back(img_path);
	}

	// Sort the images by size (Bubble sort)
    for (int i = 0; i < paths.size() - 1; i++){
        for (int j = 0; j < paths.size() - i - 1; j++){
            if (file_size(paths[j]) < file_size(paths[j + 1])) {
            	swap(paths[j], paths[j + 1]);       
            }
        }
    }

    // // Prints the list of images and sizes.
    // cout << "Sizes:" << endl;
    // for (path img : paths){
    // 	cout << string(img.stem()) << " | " << file_size(img) << endl;
    // }


    // Distribute the images in the threads.
    int img_per_reader = paths.size() / n_readers;
    int remainder = paths.size() - (n_readers * img_per_reader);
    vector<vector<path>> threads_images;

    for (int i = 0; i < n_readers; i++){
    	vector<path> thread_images;
    	for (int j = 0; j < img_per_reader; j++){
    		thread_images.push_back(paths[i + j*n_readers]);
    	}
    	threads_images.push_back(thread_images);
    }
    paths.erase(paths.begin(), paths.begin() + n_readers * img_per_reader);
    

    // Add remaining images to less busy threads.
    for (int i = n_readers-1; i > 0; i--){
    	if (paths.empty()) break;
    	threads_images[i].push_back(paths[0]);
    	paths.erase(paths.begin());
    }


    // // Print images per thread
    // cout << "Images per reader: " << img_per_reader << " remainder:" << remainder << endl;
    // for (int th = 0; th < n_readers; th++){
    // 	cout << endl << "TH" << th << ": ";
    // 	for (path img: threads_images[th]){
    // 		cout << string(img.stem()) << " ";
    // 	}
    // }
    // cout << endl;


    // Create and assign tasks to threads.
	auto filter_images = [out_dir, filters, n_threads, filter_thread](vector<path> paths){
		for (path img_path : paths) {
			ppm img(img_path);

			path out_path = out_dir; 
			out_path /= string(img_path.stem()) + ".ppm";

			apply_filters(img, out_path, filters, n_threads, filter_thread);
		}
	};

	thread threads[n_readers];
    for (int i = 0; i < n_readers; i++) threads[i] = thread(filter_images, threads_images[i]);
    for (int i = 0; i < n_readers; i++) threads[i].join();

    clock_gettime(CLOCK_REALTIME, &stop);
	double total_time;
	total_time = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	
	printf("Total Time: %lf s\n", total_time);
	return total_time;
}


int main(int argc , char* argv[]){

	const vector<string> args(argv, argv + argc);
	vector<vector<string>> filters = get_filters(args);
	string _help = help(args);
	if (!_help.empty()) cout << _help;

	if (filters.size() > 0){

		bool is_batch = batch(args);
		bool testing = test(args);
		string input = string(argv[1]);
		string output = string(argv[2]);
		unsigned int n_threads = 0;
		unsigned int n_readers = 0;

		// Defines whether to define a number of threads per filter.
		bool filter_thread = (single_thread(args) | multi_thread(args));
		if (single_thread(args)) n_threads = 1;
		else if (multi_thread(args)) n_threads = get_multi_thread(args);

		if (is_batch) n_readers = get_batch(args);

		cout << "Applying filters:"<< endl;
		for (vector<string> filter: filters) cout << "  - " << filter[0] << endl;
		cout << endl;

		// Use batch or simple loading depending on the case
		auto loader = [input, output, filters, n_threads, filter_thread](int n_readers){
			if (n_readers) return batch_load(input, output, filters, n_threads, filter_thread, n_readers);
			else return simple_load(input, output, filters, n_threads, filter_thread);
		};


		if (testing){
			auto [output_file, iterations] = get_test(args);
			ofstream myfile;
			myfile.open(output_file);
			
			for (auto i: args) myfile << i << " ";
			myfile << "\n";

			for (int i = 0; i< iterations; i++){
				double time = loader(n_readers);
				myfile << time << "\n";
			} 
		}
		else loader(n_readers);
	}

	return 0;
}