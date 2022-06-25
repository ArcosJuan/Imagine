#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


// SINGLE-THREAD FILTERS

void blackWhite(ppm& img, int nthreads);
void contrast(ppm& img, float contrast, int nthreads);
void brightness(ppm& img, float b, int nthreads);
void shades(ppm& img, unsigned char shades, int nthreads);
void merge(ppm& img1, ppm& img2, float alpha, int nthreads);
void frame(ppm& img, int width, unsigned char hue, int nthreads);
void boxBlur(ppm& img, int iterations, int nthreads);
void zoom(ppm& img, int n, int nthreads);
void edgeDetection(ppm &img, int nthreads);
void plain(ppm& img, unsigned char hue, int nthreads);
void sharpen(ppm &img, int iterations, int nthreads);

#endif