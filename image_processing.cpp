# include "image_processing.h"
# include "stb_image_write.h"

int get_average(unsigned char* image, int width, int height, int channels,int sx, int sy, int actual_w, int actual_h, int desired_channel)
{
    int avg = 0;
    for(int i = 0; i<sy; i++)
        for(int j = 0; j<sx; j++)
            if((actual_w+j)<width && (actual_h+i)<height)
                avg+=*(image+((actual_h+i)*width*channels)+((actual_w+j)*channels)+desired_channel);

    avg/=(sx*sy);
    return avg;
}
void zoom_out(unsigned char* image, int width, int height, int channels, int sx, int sy)
{
    int n_width = round((double)width/(double)sx);
    int n_heigth = round((double)height/(double)sy);
    unsigned char new_image[n_width*n_heigth*channels];
    for(int i = 0; i<n_heigth; i++)
        for(int j = 0; j<n_width; j++)
            for(int k = 0; k<channels; k++)
                new_image[(i*n_width*channels)+(channels*j)+k] = get_average(image, width, height, channels, sx, sy, j*sx, i*sy, k);

    stbi_write_jpg("image_min_graved.jpg", n_width, n_heigth, channels, new_image, n_width*channels);
}

void zoom_in(unsigned char* image, int width, int height, int channels)
{
    unsigned char new_image[4*width*height*channels];

    for(int i = 0; i<height*2; i+=2)
        for(int j = 0; j<width*2; j+=2)
            for(int k = 0; k<channels; k++)
                new_image[(width*2*i*channels)+(j*channels)+k] = *(image+(width*(i/2)*channels)+((j/2) * channels)+k);

    for(int i = 0; i<height*2; i+=2)
        for(int j = 1; j<width*2; j+=2)
            for(int k = 0; k<channels; k++)
                new_image[(width*2*i*channels)+(j*channels)+k] = (new_image[(width*2*i*channels)+((j-1)*channels)+k]+new_image[(width*2*i*channels)+((j+1)*channels)+k])/2;

    for(int i = 1; i<height*2; i+=2)
        for(int j = 0; j<width*2; j++)
            for(int k = 0; k<channels; k++)
                new_image[(width*2*i*channels)+(j*channels)+k] = (new_image[(width*2*(i-1)*channels)+(j*channels)+k]+new_image[(width*2*(i+1)*channels)+(j*channels)+k])/2;

    stbi_write_jpg("image_max_graved.jpg", width*2, height*2, channels, new_image, width*2*channels);
}

void rotate_90(unsigned char* image, int width, int height, int channels)
{
    unsigned char new_image[width*height*channels];

    for(int i = 0; i<height; i++)
        for(int j = 0; j<width; j++)
            for(int k = 0; k<channels; k++)
                new_image[(height*j*channels)+((height-1-i)*channels)+k] = *(image+(width*i*channels)+(j * channels)+k);

    stbi_write_jpg("image_rotated_graved.jpg", height, width, channels, new_image, height*channels);
}


void rotate_anti_90(unsigned char* image, int width, int height, int channels)
{
    unsigned char new_image[width*height*channels];

    for(int i = 0; i<height; i++)
        for(int j = 0; j<width; j++)
            for(int k = 0; k<channels; k++)
                new_image[(height*j*channels)+(i*channels)+k] = *(image+(width*i*channels)+(j * channels)+k);

    stbi_write_jpg("image_rotated_graved.jpg", height, width, channels, new_image, height*channels);
}



void horizontal_swap(unsigned char* image, int width, int height, int channels)
{
    for(int i = 0; i<height; i++)
        for(int j = 0; j<width/2; j++)
            std::swap_ranges(&(*(image+(width*(i+1)*channels) - j*channels)), &(*(image+(width*(i+1)*channels) - j*channels+channels)), &(*(image+(width*i*channels)+(j*channels))));
}

void vertical_swap(unsigned char* image, int width, int height, int channels)
{
    for(int j = 0; j<height/2; j++)
        std::swap_ranges(&(*(image+(j*width*channels))), &(*(image+((j+1)*width*channels))), &(*(image+((height-j-1)*width*channels))));
}

void to_gray(unsigned char* image, int width, int height, int channels)
{
    for(unsigned char *p = image; p != image + (width*height*channels); p += channels)
        std::fill(&(*p), &(*(p+channels)), (uint8_t)((*p)*0.299 + *(p + 1)*0.587 + *(p + 2)*0.114));
}

unsigned char* to_gray_copy(unsigned char* image, int width, int height, int channels)
{
    unsigned char *new_image = new  unsigned char[width*height*channels];
    for(unsigned char *p = image, *np = new_image; p != image + (width*height*channels); p += channels, np += channels)
        std::fill(&(*np), &(*(np+channels)), (uint8_t)((*p)*0.299 + *(p + 1)*0.587 + *(p + 2)*0.114));
    return new_image;
}

unsigned char* copy_image(unsigned char* image, int width, int height, int channels)
{
    unsigned char *new_image = new  unsigned char[width*height*channels];
    for(unsigned char *p = image, *np = new_image; p != image + (width*height*channels); p ++, np ++)
        *np = (*p);
    return new_image;
}

void quantize(unsigned char* image, int width, int height, int channels, int shades)
{
    int t1 = MAXS, t2 = -MAXS, tam_int;
    int quantized_values[shades];
    float tb;

    for(unsigned char *p = image; p != image + (width*height*channels); p += channels)
        t1 = std::min(t1, (int)(*p)), t2 = std::max(t2, (int)(*p));

    tam_int = t2-t1+1;

    if(shades>= tam_int)return;

    tb = tam_int/(1.0*shades);

    for(int i = 0; i<shades; i++)
    {
        float lower_b = t1-0.5+tb*i;
        float quantized_value = (lower_b*2+tb)/2.0;
        quantized_values[i] = (int)quantized_value;
    }

    for(unsigned char *p = image; p != image + (width*height*channels); p += channels)
    {
        int index = ((*p)-t1)/tb;
        std::fill(&(*p), &(*(p+channels)),quantized_values[index]);
    }
}

void histogram_computation(unsigned char* image, int width, int height, int channels, int *histogram)
{
    for(int i = 0; i<=255; i++)
        histogram[i] = 0;

    for(unsigned char *p = image; p != image + (width*height*channels); p += channels)
        histogram[(*p)]++;
}

void cumulative_histogram_computation(int *histogram, int *hist_cum, float scaling_factor)
{
    hist_cum[0] = scaling_factor * histogram[0];
    for(int i = 1; i<=255; i++)
        hist_cum[i] = hist_cum[i-1] + (scaling_factor * histogram[i]);
}


void histogram_normalization(int *histogram)
{
    int high = histogram[0];
    for(int i = 1; i<256; i++)
        high = std::max(high, histogram[i]);

    for(int i = 0; i<256; i++)
        histogram[i] = histogram[i]*255/high;
}
void histogram_print(int *histogram)
{
    for(int i = 0; i<=255; i++)
        printf("%d = %d\n", i, histogram[i]);
}

void adjust_brightness(unsigned char* image, int width, int height, int channels, int value)
{
    for(unsigned char *p = image; p != image + (width*height*channels); p ++)
        *p = std::min(std::max((*p)+value,0),255);
}


void adjust_contrast(unsigned char* image, int width, int height, int channels, double value)
{
    for(unsigned char *p = image; p != image + (width*height*channels); p ++)
        *p = std::min(std::max((int)((*p)*value),0),255);
}

void negative_computation(unsigned char* image, int width, int height, int channels)
{
    for(unsigned char *p = image; p != image + (width*height*channels); p ++)
        *p = 255-(*p);
}

void histogram_equalization(unsigned char* image, int width, int height, int channels)
{
    int histogram[256], hist_cum[256];
    float alpha = 255.0 / (width*height*1.0);
    unsigned char *gray_image;
    gray_image = to_gray_copy(image, width, height, channels);
    histogram_computation(gray_image,width, height,channels, histogram);
    cumulative_histogram_computation(histogram, hist_cum, alpha);
    histogram_normalization(hist_cum);

    for(unsigned char *p = image; p != image + (width*height*channels); p ++)
        *p =hist_cum[(*p)];

}

void convolution_image_ignoring_the_borders (unsigned char* image, int width, int height, int channels, double kernel[3][3], bool needsum)
{

    unsigned char* aux_image = copy_image(image, width, height, channels);
    float sum;

    for(int i = 1; i < height - 1; i++)
        for(int j = 1; j < width - 1; j++)
            for(int channel = 0; channel<channels; channel++)
            {
                sum = 0.0;

                for(int l = -1; l <= 1; l++)
                    for(int k = -1 ; k<=1; k++)
                        sum+=*(aux_image+(width*(i+l)*channels)+((j+k) * channels)+channel) * kernel[2-(1+k)][2-(1+l)];

                if(needsum)
                    sum+=127;

                if(sum<0)
                    sum = 0;
                else if(sum>255)
                    sum = 255;

                *(image+(i*width*channels)+(j*channels)+channel) =  sum;
            }

}
