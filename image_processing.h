#include <bits/stdc++.h>

#define CHANNEL_NUM 3
#define MAXS 1000

void horizontal_swap(unsigned char* image, int width, int height, int channels);
void vertical_swap(unsigned char* image, int width, int height, int channels);
void to_gray(unsigned char* image, int width, int height, int channels);
void quantize(unsigned char* image, int width, int height, int channels, int shades);
void histogram_computation(unsigned char* image, int width, int height, int channels, int *histogram);
void adjust_brightness(unsigned char* image, int width, int height, int channels, int value);
void adjust_contrast(unsigned char* image, int width, int height, int channels, double value);
void histogram_print(int *histogram);
void negative_computation(unsigned char* image, int width, int height, int channels);
void zoom_out(unsigned char* image, int width, int height, int channels, int sx, int sy);
void zoom_in(unsigned char* image, int width, int height, int channels);
void rotate_90(unsigned char* image, int width, int height, int channels);
void rotate_anti_90(unsigned char* image, int width, int height, int channels);
void histogram_equalization(unsigned char* image, int width, int height, int channels);
void convolution_image_ignoring_the_borders (unsigned char* image, int width, int height, int channels, double kernel[3][3], bool needsum);
