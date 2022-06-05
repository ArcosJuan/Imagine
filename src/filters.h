#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


// SINGLE-THREAD FILTERS

void blackWhite(ppm& img);
void contrast(ppm& img, float contrast);
void brightness(ppm& img, float b);
void shades(ppm& img, unsigned char shades);
void merge(ppm& img1, ppm& img2, float alpha);
void frame(ppm& img, int width, pixel color = pixel());
void boxBlur(ppm &img, int iterations);
void zoom(ppm &img, int n);
void edgeDetection(ppm &img);
void plain(ppm &img, unsigned char c = 0);

// MULTI-THREAD FILTERS

#endif