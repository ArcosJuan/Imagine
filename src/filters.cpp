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


void contrast(ppm& img, float contrast) {
	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++) {
			float f = (259*(contrast+255)) / (255*(259-contrast));

			// (f*(r-128)+128, f*(b-128)+128, f*(g-128)+128).
			img.setPixel(i,j,img.getPixel(i,j).sub(128).mult(f).add(128).truncate());
		}
}


ppm merge(ppm& img1, ppm& img2, float alpha) {
	// TODO: manage exceptions  (apha <= 1 && apha >= 0).

	// Create an image as tall as the tallest image and as wide as the widest image.
	int height = (img1.height>img2.height) ? img1.height : img2.height;
	int width = (img1.width>img2.width) ? img1.width : img2.width;
	ppm out_img(width, height);

	for(int i = 0; i < out_img.height; i++)
		for(int j = 0; j < out_img.width; j++) {
			if ((img1.height<=i || img1.width<=j) && (img2.height<=i || img2.width<=j))
				// If none of the images occupy the pixel sets the pixel to white.
				out_img.setPixel(i,j, pixel{255,255,255});
			else {
				// If the images don't overlap in a pixel sets the pixel of the image that occupies the pixel.
				pixel img1_pixel = (img1.height<=i || img1.width<=j) ?  img2.getPixel(i,j) : img1.getPixel(i,j);
				pixel img2_pixel = (img2.height<=i || img2.width<=j) ? img1.getPixel(i,j) : img2.getPixel(i,j);

				// (r1*p1 + r2*p2, g1*p1 + g2*p2, b1*p1 + b2*p2).
				// p1=alpha, p2=1-alpha.
				out_img.setPixel(i,j, img1_pixel.mult(alpha).addp(img2_pixel.mult(1-alpha)));
			}
		}
	return out_img;
}