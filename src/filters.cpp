#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic>  

using namespace std;

void plain(ppm& img, unsigned char c) {
	for(int y = 0; y< img.height; y++){
		for(int x = 0; x < img.width; x++){ 
			img.setPixel(y, x, pixel(c,c,c));
		}
	}
}


void blackWhite(ppm& img) { shades(img, 255); }


void shades(ppm& img, unsigned char shades) {
	// TODO: manage exceptions  (shades < 1).

	float range = 255/(shades-1);
	for(int y = 0; y < img.height; y++){
		for(int x = 0; x < img.width; x++) {
			int grey_tone = img.getPixel(y,x).cumsum()/3/range;
			grey_tone *= range;
			img.setPixel(y, x, pixel(grey_tone,grey_tone,grey_tone));
		}
	}
}


void brightness(ppm& img, float b){
	// TODO: manage exceptions  ( -1 > b > 1).
	for(int y = 0; y < img.height; y++){
		for(int x = 0; x < img.width; x++) {
			img.setPixel(y, x, img.getPixel(y,x).add(255*b).truncate());
		}
	}
}


void contrast(ppm& img, float contrast) {
	for(int y = 0; y < img.height; y++){
		for(int x = 0; x < img.width; x++) {
			float f = (259*(contrast+255)) / (255*(259-contrast));

			// (f*(r-128)+128, f*(b-128)+128, f*(g-128)+128).
			img.setPixel(y,x,img.getPixel(y,x).sub(128).mult(f).add(128).truncate());
		}
	}
}


void merge(ppm& img1, ppm& img2, float alpha) {
	// TODO: manage exceptions  (apha <= 1 && apha >= 0).

	int height = (img1.height>img2.height) ? img2.height : img1.height;
	int width = (img1.width>img2.width) ? img2.width : img1.width;
	ppm out_img(width, height);

	for(int y= 0; y< out_img.height; y++) {
		for(int x = 0; x < out_img.width; x++) {
			// (r1*p1 + r2*p2, g1*p1 + g2*p2, b1*p1 + b2*p2).
			// p1=alpha, p2=1-alpha.
			out_img.setPixel(y,x, img1.getPixel(y,x).mult(alpha).addp(img2.getPixel(y,x).mult(1-alpha)));
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


void edgeDetection(ppm &img) {
	shades(img, 8);
	boxBlur(img, 5);
	int kernel_x[3][3] = {
		{1,0,-1},
		{2,0,-2},
		{1,0,-1}
	};
	int kernel_y[3][3] = {
		{1, 2, 1},
		{0, 0, 0},
		{-1, -2, -1}
	};
	ppm img_x = img;
	ppm img_y = img;
	
	convolution_filter(img_x, kernel_x);	
	convolution_filter(img_y, kernel_y);

	for(int y = 1; y < img.height-4; y++) {
		for(int x = 1; x < img.width-4; x++) {
			pixel pixel_x = img_x.getPixel(y,x);
			pixel pixel_y = img_y.getPixel(y,x);
			
			pixel_x = pixel_x.power(2);
			pixel_y = pixel_y.power(2);
			pixel result = pixel_x;
			
			result = result.addp(pixel_y).power(((float)1/2));
			img.setPixel(y, x, result);
		}
	}
}