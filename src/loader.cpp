#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include <filesystem>
#include "filters.h"
#define ONE_OVER_BILLION 1E-9


using namespace std;
using namespace filesystem;

int main(int argc , char* argv[]){

	if(string(argv[1]) == "--help" || string(argv[1]) == "-h"){
		cout << "Usage: ./loader <filter> <p1> <n_threads> <root_dir> <output_dir>" << endl;
		return 0; 
	}
	
	string filter = string(argv[1]);
	float p1 = atof(argv[2]);
	unsigned int n = atoi(argv[3]);
	string dir = string(argv[4]);
	path out_dir = path(dir);
	if(argv[5]) out_dir = path(string(argv[5]));

	// Create the directory if it doesn't exist.
	if (!is_directory(out_dir)) create_directory(out_dir);

	
    for (const auto & image : directory_iterator(dir)){
		auto img_path = image.path();
		if (img_path.extension() != ".ppm") continue;

        ppm img(img_path);
		if (filter == "plain") plain(img, (unsigned char)p1);
		else if (filter == "blackwhite") blackWhite(img);
		else if (filter == "shades") shades(img, (unsigned char)p1);
		else if (filter == "contrast") contrast(img, (float)p1);
		else if (filter == "bright") brightness(img, (float)p1);
		else if (filter == "frame") frame(img, (int)p1);
		else if (filter == "zoom") zoom(img, (int)p1);
		else if (filter == "blur") boxBlur(img, p1);	
		else if (filter == "edge") edgeDetection(img);	
	
		path out_path = out_dir; 
		out_path /= string(img_path.stem()) + "_" + filter + ".ppm";
		img.write(out_path);	
	}	
	cout << "DONE!" << endl;
	return 0;
}
