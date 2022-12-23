#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale

void blur_pixel(int row, int column, RGBTRIPLE pixel, RGBTRIPLE image);

void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int pixel = 0; pixel < width; pixel++)
        {
            int summed_colors = image[row][pixel].rgbtRed + image[row][pixel].rgbtGreen + image[row][pixel].rgbtBlue;
            int average_gray_scale = round(summed_colors / 3.0);
            image[row][pixel].rgbtRed = average_gray_scale;
            image[row][pixel].rgbtGreen = average_gray_scale;
            image[row][pixel].rgbtBlue = average_gray_scale;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    void swap_it(RGBTRIPLE *x, RGBTRIPLE *y);
    for (int row = 0; row < height; row++)
    {
        for (int pixel = 0; pixel < (int)width/2; pixel++)
        {
            swap_it(&image[row][pixel], &image[row][width - pixel - 1]);
        }
    }
    return;
}


RGBTRIPLE dummy = {.rgbtRed = 0, .rgbtGreen = 0, .rgbtBlue = 0};

RGBTRIPLE top, bottom;
RGBTRIPLE left, top_left, bottom_left;
RGBTRIPLE right, top_right, bottom_right;

BYTE red_total, green_total, blue_total;
// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_buffer[height][width];

    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            image_buffer[r][p] = image[r][p];
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int pixel = 0; pixel < width; pixel++)
        {
            top = dummy, bottom = dummy;
            left = dummy, top_left = dummy, bottom_left = dummy;
            right = dummy, top_right = dummy, bottom_right = dummy;

            int has_top = 0;
            int has_bottom = 0;

            double steps = 1.00;


            if (row - 1 >= 0)
            {
                bottom = image[row-1][pixel];
                has_bottom = 1;
                steps++;
            }

            if (row + 1 < height)
            {
                top = image[row+1][pixel];
                has_top = 1;
                steps++;
            }

            if (pixel - 1 >= 0)
            {
                left = image[row][pixel-1];
                steps++;
                if (has_top)
                {
                    top_left = image[row+1][pixel-1];
                    steps++;
                }
                if (has_bottom)
                {
                    bottom_left = image[row-1][pixel-1];
                    steps++;
                }
            }

            if (pixel + 1 < width)
            {
                right = image[row][pixel+1];
                steps++;
                if (has_top)
                {
                    top_right = image[row+1][pixel+1];
                    steps++;
                }
                if(has_bottom)
                {
                    bottom_right = image[row-1][pixel+1];
                    steps++;
                }
            }

            int red_sum, green_sum, blue_sum;
            int avg_red, avg_green, avg_blue;

            red_sum = top.rgbtRed + bottom.rgbtRed + left.rgbtRed + right.rgbtRed + top_left.rgbtRed + top_right.rgbtRed + bottom_left.rgbtRed + bottom_right.rgbtRed + image[row][pixel].rgbtRed;
            green_sum = top.rgbtGreen + bottom.rgbtGreen + left.rgbtGreen + right.rgbtGreen + top_left.rgbtGreen + top_right.rgbtGreen + bottom_left.rgbtGreen + bottom_right.rgbtGreen + image[row][pixel].rgbtGreen;
            blue_sum = top.rgbtBlue + bottom.rgbtBlue + left.rgbtBlue + right.rgbtBlue + top_left.rgbtBlue + top_right.rgbtBlue + bottom_left.rgbtBlue + bottom_right.rgbtBlue + image[row][pixel].rgbtBlue;

            avg_red = (int)round(red_sum / steps);
            avg_green = (int)round(green_sum / steps);
            avg_blue = (int)round(blue_sum / steps);

            has_top = 0;
            has_bottom = 0;

            image_buffer[row][pixel].rgbtRed = round(avg_red);
            image_buffer[row][pixel].rgbtGreen = round(avg_green);
            image_buffer[row][pixel].rgbtBlue = round(avg_blue);
        }
    }

    for(int r = 0; r < height; r++)
    {
        for(int p = 0; p < width; p++)
        {
            image[r][p] = image_buffer[r][p];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_buffer[height][width];

    int gx_kernel[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gy_kernel[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    for(int r = 0; r < height; r++)
    {
        for(int p = 0; p < width; p++)
        {
            image_buffer[r][p] = image[r][p];
        }
    }

    for(int row = 0; row < height; row++)
    {
        for(int pixel = 0; pixel < width; pixel++)
        {
            RGBTRIPLE current_pixel = image[row][pixel];

            int has_top = 0;
            int has_bottom = 0;
            /*
                    0            1          2
                {top_left       top      top_right
                left           middle    right
                bottom_left    bottom    bottom_right}
            */
            int red_gx = 0; int green_gx = 0; int blue_gx = 0;
            int red_gy = 0; int green_gy = 0; int blue_gy = 0;

            red_gx += current_pixel.rgbtRed * gx_kernel[4];
            green_gx += current_pixel.rgbtGreen * gx_kernel[4];
            blue_gx += current_pixel.rgbtBlue * gx_kernel[4];

            red_gy += current_pixel.rgbtRed * gy_kernel[4];
            green_gy += current_pixel.rgbtGreen * gy_kernel[4];
            blue_gy += current_pixel.rgbtBlue * gy_kernel[4];

            //Check top existance.
            if(row + 1 < height)
            {
                RGBTRIPLE top_pixel = image[row + 1][pixel];
                has_top = 1;
                red_gx += top_pixel.rgbtRed * gx_kernel[1];
                green_gx += top_pixel.rgbtGreen * gx_kernel[1];
                blue_gx += top_pixel.rgbtBlue * gx_kernel[1];

                red_gy += top_pixel.rgbtRed * gy_kernel[1];
                green_gy += top_pixel.rgbtGreen * gy_kernel[1];
                blue_gy += top_pixel.rgbtBlue * gy_kernel[1];
            }
            //Check bottom existance.
            if(row - 1 >= 0)
            {
                RGBTRIPLE bottom_pixel = image[row - 1][pixel];
                has_bottom = 1;
                red_gx += bottom_pixel.rgbtRed * gx_kernel[7];
                green_gx += bottom_pixel.rgbtGreen * gx_kernel[7];
                blue_gx += bottom_pixel.rgbtBlue * gx_kernel[7];

                red_gy += bottom_pixel.rgbtRed * gy_kernel[7];
                green_gy += bottom_pixel.rgbtGreen * gy_kernel[7];
                blue_gy += bottom_pixel.rgbtBlue * gy_kernel[7];
            }
            //Check right existance.
            if(pixel + 1 < width)
            {
                RGBTRIPLE right_pixel = image[row][pixel + 1];
                red_gx += right_pixel.rgbtRed * gx_kernel[5];
                green_gx += right_pixel.rgbtGreen * gx_kernel[5];
                blue_gx += right_pixel.rgbtBlue * gx_kernel[5];

                red_gy += right_pixel.rgbtRed * gy_kernel[5];
                green_gy += right_pixel.rgbtGreen * gy_kernel[5];
                blue_gy += right_pixel.rgbtBlue * gy_kernel[5];
                if(has_top)
                {
                    RGBTRIPLE top_right_pixel = image[row + 1][pixel + 1];
                    red_gx += top_right_pixel.rgbtRed * gx_kernel[2];
                    green_gx += top_right_pixel.rgbtGreen * gx_kernel[2];
                    blue_gx += top_right_pixel.rgbtBlue * gx_kernel[2];

                    red_gy += top_right_pixel.rgbtRed * gy_kernel[2];
                    green_gy += top_right_pixel.rgbtGreen * gy_kernel[2];
                    blue_gy += top_right_pixel.rgbtBlue * gy_kernel[2];
                }

                if(has_bottom)
                {
                    RGBTRIPLE bottom_right_pixel = image[row - 1][pixel + 1];
                    red_gx += bottom_right_pixel.rgbtRed * gx_kernel[8];
                    green_gx += bottom_right_pixel.rgbtGreen * gx_kernel[8];
                    blue_gx += bottom_right_pixel.rgbtBlue * gx_kernel[8];

                    red_gy += bottom_right_pixel.rgbtRed * gy_kernel[8];
                    green_gy += bottom_right_pixel.rgbtGreen * gy_kernel[8];
                    blue_gy += bottom_right_pixel.rgbtBlue * gy_kernel[8];
                }
            }
            //Check left existance.
            if(pixel - 1 >= 0)
            {
                RGBTRIPLE left_pixel = image[row][pixel - 1];
                red_gx += left_pixel.rgbtRed * gx_kernel[3];
                green_gx += left_pixel.rgbtGreen * gx_kernel[3];
                blue_gx += left_pixel.rgbtBlue * gx_kernel[3];

                red_gy += left_pixel.rgbtRed * gy_kernel[3];
                green_gy += left_pixel.rgbtGreen * gy_kernel[3];
                blue_gy += left_pixel.rgbtBlue * gy_kernel[3];

                if(has_top)
                {
                    RGBTRIPLE top_left_pixel = image[row + 1][pixel - 1];
                    red_gx += top_left_pixel.rgbtRed * gx_kernel[0];
                    green_gx += top_left_pixel.rgbtGreen * gx_kernel[0];
                    blue_gx += top_left_pixel.rgbtBlue * gx_kernel[0];

                    red_gy += top_left_pixel.rgbtRed * gy_kernel[0];
                    green_gy += top_left_pixel.rgbtGreen * gy_kernel[0];
                    blue_gy += top_left_pixel.rgbtBlue * gy_kernel[0];
                }

                if(has_bottom)
                {
                    RGBTRIPLE bottom_left_pixel = image[row - 1][pixel -1];
                    red_gx += bottom_left_pixel.rgbtRed * gx_kernel[6];
                    green_gx += bottom_left_pixel.rgbtGreen * gx_kernel[6];
                    blue_gx += bottom_left_pixel.rgbtBlue * gx_kernel[6];

                    red_gy += bottom_left_pixel.rgbtRed * gy_kernel[6];
                    green_gy += bottom_left_pixel.rgbtGreen * gy_kernel[6];
                    blue_gy += bottom_left_pixel.rgbtBlue * gy_kernel[6];
                }
            }

            image_buffer[row][pixel].rgbtRed = round(sqrt((red_gx * red_gx) + (red_gy * red_gy)));
            if (round(sqrt((red_gx * red_gx) + (red_gy * red_gy))) > 255)
            {
                image_buffer[row][pixel].rgbtRed = 255;
            }
            else
            {
                image_buffer[row][pixel].rgbtRed = round(sqrt((red_gx * red_gx) + (red_gy * red_gy)));
            }
            if (round(sqrt((green_gx * green_gx) + (green_gy * green_gy))) > 255)
            {
                image_buffer[row][pixel].rgbtGreen = 255;
            }
            else
            {
                image_buffer[row][pixel].rgbtGreen = round(sqrt((green_gx * green_gx) + (green_gy * green_gy)));
            }
            if (round(sqrt((blue_gx * blue_gx) + (blue_gy * blue_gy))) > 255)
            {
                image_buffer[row][pixel].rgbtBlue = 255;
            }
            else
            {
                image_buffer[row][pixel].rgbtBlue = round(sqrt((blue_gx * blue_gx) + (blue_gy * blue_gy)));
            }
        }
    }

    for(int r = 0; r < height; r++)
    {
        for(int p = 0; p < width; p++)
        {
            image[r][p] = image_buffer[r][p];
        }
    }

    return;
}

void swap_it(RGBTRIPLE *x, RGBTRIPLE *y)
{
    RGBTRIPLE temp_buffer = *x;
    *x = *y;
    *y = temp_buffer;
}

