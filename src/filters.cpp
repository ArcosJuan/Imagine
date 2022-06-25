#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic>

using namespace std;


template<typename T>
void multi_threads(ppm& img, int nthreads, T thread_function) {
	int h = img.height / nthreads;
	thread threads[nthreads];

	for (int i = 0; i < nthreads; i++) {
		if (i == nthreads-1) {
			// start = i*h; end = img.height;
			threads[i] = thread_function(i*h, img.height);
			break;	
		} 
		// start = i*h; end = i*h+h;
		threads[i] = thread_function(i*h, i*h+h);
	}
	
	for (int i = 0; i < nthreads; i++) threads[i].join();
}


#pragma region _pixel2pixel_filters
void _plain(ppm& img, unsigned char hue, int y_start=0, int y_end=0) {
	if (y_end == 0) y_end = img.height;
	for(int y = y_start; y < y_end; y++){
		for(int x = 0; x < img.width; x++) {
			img.setPixel(y, x, pixel(hue,hue,hue));
		}
	}
}



void _shades(ppm& img, unsigned char shades, int y_start=0, int y_end=0) {
	// TODO: manage exceptions  (shades < 1).

	float range = 255/(shades-1);
	if (y_end == 0) y_end = img.height;
	for(int y = y_start; y < y_end; y++){
		for(int x = 0; x < img.width; x++) {
			int grey_tone = img.getPixel(y,x).cumsum()/3/range;
			grey_tone *= range;
			img.setPixel(y, x, pixel(grey_tone,grey_tone,grey_tone));
		}
	}
}


void _brightness(ppm& img, float b, int y_start=0, int y_end=0) {
	// TODO: manage exceptions  ( -1 > b > 1).
	if (y_end == 0) y_end = img.height;
	for(int y = y_start; y < y_end; y++){
		for(int x = 0; x < img.width; x++) {
			img.setPixel(y, x, img.getPixel(y,x).add(255*b).truncate());
		}
	}
}


void _contrast(ppm& img, float contrast, int y_start=0, int y_end=0) {
	if (y_end == 0) y_end = img.height;
	for(int y = y_start; y < y_end; y++){
		for(int x = 0; x < img.width; x++) {
			float f = (259*(contrast+255)) / (255*(259-contrast));

			// (f*(r-128)+128, f*(b-128)+128, f*(g-128)+128).
			img.setPixel(y,x,img.getPixel(y,x).sub(128).mult(f).add(128).truncate());
		}
	}
}


void _frame(ppm& img, int width, unsigned char hue, int y_start=0, int y_end=0){
	if (y_end == 0) y_end = img.height;
	for(int y = y_start; y < y_end; y++){
		for(int x = 0; x < img.width; x++) {
			if (y <= width | y >= img.height - width | x <= width  | x >= img.width - width) {
				img.setPixel(y, x, pixel(hue,hue,hue));
			}
		}
	}
}


void _merge(ppm& img1, ppm& img2, ppm& out_img, float alpha, int y_start=0, int y_end=0) {
	// TODO: manage exceptions  (apha <= 1 && apha >= 0).
	if (y_end == 0) y_end = out_img.height;
	for(int y = y_start; y < y_end; y++){
		for(int x = 0; x < out_img.width; x++) {
			// (r1*p1 + r2*p2, g1*p1 + g2*p2, b1*p1 + b2*p2).
			// p1=alpha, p2=1-alpha.
			out_img.setPixel(y,x, img1.getPixel(y,x).mult(alpha).addp(img2.getPixel(y,x).mult(1-alpha)));
		}
	}
}


void _zoom(ppm& img, ppm& out_img, int n, int y_start=0, int y_end=0){
	if (y_end == 0) y_end = img.height;
	for(int y = y_start; y < y_end; y++){
		for(int x = 0; x < img.width; x++) {
			pixel p = img.getPixel(y,x);

			for(int i = 0; i < n; i++) {
				for(int j = 0; j < n; j++) {
					out_img.setPixel(y*n + i, x*n + j, p);
				}
			}
		}
	}
}
#pragma endregion _pixel2pixel_filters


#pragma region pixel2pixel_filters
void plain(ppm& img, unsigned char hue, int nthreads) {
	if (nthreads != 1) {
		auto foo = [&img, hue](int y_start, int y_end){
			return thread(_plain, ref(img), hue, y_start, y_end);
		};
		multi_threads(img, nthreads, foo);
	}
	else _plain(img, hue);
}


void shades(ppm& img, unsigned char scale, int nthreads) {
	if (nthreads != 1) {
		auto foo = [&img, scale](int y_start, int y_end){
			return thread(_shades, ref(img), scale, y_start, y_end);
		};

		multi_threads(img, nthreads, foo);
	}
	else _shades(img, scale);
}


void blackWhite(ppm& img, int nthreads) { shades(img, 255, nthreads); }


void contrast(ppm& img, float contrast, int nthreads) {
	if (nthreads != 1) {
		auto foo = [&img, contrast](int y_start, int y_end){
			return thread(_contrast, ref(img), contrast, y_start, y_end);
		};

		multi_threads(img, nthreads, foo);
	}
	else _contrast(img, contrast);
}


void brightness(ppm& img, float b, int nthreads) {
	if (nthreads != 1) {
		auto foo = [&img, b](int y_start, int y_end){
			return thread(_brightness, ref(img), b, y_start, y_end);
		};

		multi_threads(img, nthreads, foo);
	}
	else _brightness(img, b);
}


void merge(ppm& img1, ppm& img2, float alpha, int nthreads) {
	int height = (img1.height>img2.height) ? img2.height : img1.height;
	int width = (img1.width>img2.width) ? img2.width : img1.width;
	ppm out_img(width, height);

	if (nthreads != 1) {
		auto foo = [&out_img, &img1, &img2, alpha](int y_start, int y_end){
			return thread(_merge, ref(img1), ref(img2), ref(out_img), alpha, y_start, y_end);
		};

		multi_threads(out_img, nthreads, foo);
	}
	else _merge(img1, img2, out_img, alpha);

	img1 = out_img;
}


void frame(ppm& img, int width, unsigned char hue, int nthreads) {
	if (nthreads != 1) {
		auto foo = [&img, width, hue](int y_start, int y_end){
			return thread(_frame, ref(img), width, hue, y_start, y_end);
		};
		
		multi_threads(img, nthreads, foo);
	}
	else _frame(img, width, hue);
}


void zoom(ppm &img, int n, int nthreads){
	ppm out_img (img.width * n, img.height * n);

	if (nthreads != 1) {
		auto foo = [&img, &out_img, n](int y_start, int y_end){
			return thread(_zoom, ref(img), ref(out_img), n, y_start, y_end);
		};
		multi_threads(img, nthreads, foo);
	}
	else _zoom(img, out_img, n);

	img = out_img;
}


#pragma endregion pixel2pixel_filters


#pragma region convolution_filters
void _convolution_filter(ppm& img, ppm& out_img, float kernel[3][3], bool truncate, int y_start=0, int y_end=0) {
	if (y_end == 0 || y_end == img.height) y_end = img.height-1;
	if (y_start == 0) y_start = 1;
	for(int y = y_start; y < y_end; y++){
		for(int x = 1; x < img.width-1; x++) {
			pixel summation;

			for(int i = -1; i <=1; i++) {
				for(int j = -1; j <=1; j++) {
					summation.addp(img.getPixel(i+y, j+x).mult(kernel[i+1][j+1])); 
				}
			}
			if (truncate) summation.truncate();
			out_img.setPixel(y-1,x-1, summation);
		}
	}
}


void boxBlur(ppm &img, int iterations, int nthreads) {
	float kernel[3][3] = {
		{1/9.f,1/9.f,1/9.f},
		{1/9.f,1/9.f,1/9.f},
		{1/9.f,1/9.f,1/9.f}
	};
	
	for (int i=0; i<iterations; i++) {	
		ppm out_img(img.width-2, img.height-2);

		if (nthreads != 1) {
			auto foo = [&img, &out_img, &kernel](int y_start, int y_end){
				return thread(_convolution_filter, ref(img), ref(out_img), kernel, true, y_start, y_end);
			};
			multi_threads(img, nthreads, foo);
		}
		else _convolution_filter(img, out_img, kernel, true);

		img = out_img;
	}
}


void sharpen(ppm &img, int iterations, int nthreads) {
	float kernel[3][3] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}
	};
	
	for (int i=0; i<iterations; i++) {	
		ppm out_img(img.width-2, img.height-2);
		
		if (nthreads != 1) {
			auto foo = [&img, &out_img, &kernel](int y_start, int y_end){
				return thread(_convolution_filter, ref(img), ref(out_img), kernel, y_start, y_end, true);
			};
			multi_threads(img, nthreads, foo);
		}
		else _convolution_filter(img, out_img, kernel, true);

		img = out_img;
	}
}


void edgeDetection(ppm &img, int nthreads) {
	blackWhite(img, nthreads);
	boxBlur(img, 1, nthreads);

	float kernel_x[3][3] = {
		{1,0,-1},
		{2,0,-2},
		{1,0,-1}
	};
	float kernel_y[3][3] = {
		{1, 2, 1},
		{0, 0, 0},
		{-1, -2, -1}
	};

	ppm img_x = img;
	ppm img_y = img;


	if (nthreads != 1) {
		auto foo_x = [&img, &img_x, &kernel_x](int y_start, int y_end){
				return thread(_convolution_filter, ref(img), ref(img_x), kernel_x, false, y_start, y_end);
		};
		auto foo_y = [&img, &img_y, &kernel_y](int y_start, int y_end){
				return thread(_convolution_filter, ref(img), ref(img_y), kernel_y, false, y_start, y_end);
		};

		multi_threads(img, nthreads, foo_x);
		multi_threads(img, nthreads, foo_y);
	}
	else {
		_convolution_filter(img, img_x, kernel_x, false);
		_convolution_filter(img, img_y, kernel_y, false);
	}
	

	for(int y = 1; y < img.height-4; y++) {
		for(int x = 1; x < img.width-4; x++) {
			pixel pixel_x = img_x.getPixel(y,x);
			pixel pixel_y = img_y.getPixel(y,x);

			float px = pow(pixel_x.r ,2);
			float py = pow(pixel_y.r, 2);
			int pr = (int)sqrt(px + py);

			pixel result(pr,pr,pr);
			img.setPixel(y, x, result.truncate());
		}
	}
}
#pragma endregion convolution_filters
