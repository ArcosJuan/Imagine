#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic>  

#define BLACK 0

using namespace std;

void plain(ppm& img, unsigned char c) {
	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++)			
			img.setPixel(i, j, pixel(c,c,c));
}


void blackWhite(ppm& img) { shades(img, 255); }


void shades(ppm& img, unsigned char shades) {
	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++) {
			unsigned char grey_tone = (img.getPixel(i,j).cumsum()/3) / (255/(shades-1));
			img.setPixel(i, j, pixel(grey_tone,grey_tone,grey_tone));
		}
}
