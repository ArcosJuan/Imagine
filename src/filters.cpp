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

void plain(ppm& img, unsigned char c = BLACK) {
	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++)			
			img.setPixel(i, j, pixel(c,c,c));
}


void blackWhite(ppm& img) { shades(img, 255); }


void shades(ppm& img, unsigned char shades) {
	//TODO: Validate that shades is grater than one.
	float range = 255/(shades-1);
	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++) {
			int grey_tone = img.getPixel(i,j).cumsum()/3/range;
			grey_tone *= range;
			img.setPixel(i, j, pixel(grey_tone,grey_tone,grey_tone));
		}
}


void contrast(ppm& img, float contrast) {
	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++) {
			float f = (259*(contrast+255)) / (255*(259-contrast));

			// (f*(r-128)+128, f*(b-128)+128, f*(g-128)+128).
			img.setPixel(i,j,img.getPixel(i,j).sub(128).mult(f).add(128).truncate());
		}
}


void merge(ppm& img1, ppm& img2, float alpha) {
	// TODO: manage exceptions  (apha <= 1 && apha >= 0).

	int height = (img1.height>img2.height) ? img2.height : img1.height;
	int width = (img1.width>img2.width) ? img2.width : img1.width;
	ppm out_img(width, height);

	for(int i = 0; i < out_img.height; i++) {
		for(int j = 0; j < out_img.width; j++) {
			// (r1*p1 + r2*p2, g1*p1 + g2*p2, b1*p1 + b2*p2).
			// p1=alpha, p2=1-alpha.
			out_img.setPixel(i,j, img1.getPixel(i,j).mult(alpha).addp(img2.getPixel(i,j).mult(1-alpha)));
		}
	}
	img1 = out_img;
}


void convolution_filter(ppm& img, int kernel[3][3]) {
	ppm out_img (img.width-2, img.height-2);
		
	for(int y = 1; y < img.height-1; y++) {
		for(int x = 1; x < img.width-1; x++) {
			pixel summation;
			for(int i = -1; i <=1; i++) {
				for(int j = -1; j <=1; j++) {
					summation.addp(img.getPixel(i+y, j+x).mult(kernel[i+1][j+1])); 
				}
			}
			out_img.setPixel(y-1,x-1, summation.mult((float)1/9).truncate());
		}
	}
	img = out_img;
}


void boxBlur(ppm &img, int iterations=1) {
	int kernel[3][3] = {
		{1,1,1},
		{1,1,1},
		{1,1,1}
	};
	for (int i=0; i<=iterations; i++) convolution_filter(img, kernel);
}

}