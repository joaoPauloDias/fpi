#include <bits/stdc++.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "image_processing.h"

using namespace std;


int main()
{
    int width, height, bpp;
    int histogram[256];
    double kernel[3][3] = {{0, -1, 0},{-1, -4, -1},{0, -1, 0}};
    double kernel2[3][3] = {{0.0625, 0.125, 0.0625},{0.125, 0.25, 0.125},{0.0625, 0.125, 0.0625}};

    unsigned char* rgb_image = stbi_load("C:\\Users\\ayala\\Desktop\\Chateau_small.jpg", &width, &height, &bpp, 3);
    //stbi_image_free(rgb_image);

    //to_gray(rgb_image,width, height,CHANNEL_NUM);

    //quantize(rgb_image,width, height,CHANNEL_NUM, 2);
    //negative_computation(rgb_image,width, height,CHANNEL_NUM);
    //histogram_computation(rgb_image,width, height,CHANNEL_NUM, histogram);
    //histogram_print(histogram);
    //rotate_90(rgb_image,width, height,CHANNEL_NUM);
    //rotate_anti_90(rgb_image,width, height,CHANNEL_NUM);
    //histogram_equalization(rgb_image,width, height,CHANNEL_NUM);
    convolution_image_ignoring_the_borders(rgb_image,width, height,CHANNEL_NUM,kernel2, false);


    stbi_write_jpg("image_graved.jpg", width, height, CHANNEL_NUM, rgb_image, width*CHANNEL_NUM);
    return 0;
}
