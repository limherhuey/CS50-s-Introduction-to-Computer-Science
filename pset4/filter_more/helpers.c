#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int ave;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            ave = round(((float) image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3);
            image[i][j].rgbtRed = ave;
            image[i][j].rgbtGreen = ave;
            image[i][j].rgbtBlue = ave;
        }
    }
    return;
}

//Helper function for reflect
void refl(BYTE *left, BYTE *right)
{
    BYTE temp = *left;
    *left = *right;
    *right = temp;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            refl(&image[i][j].rgbtRed, &image[i][width - j - 1].rgbtRed);
            refl(&image[i][j].rgbtGreen, &image[i][width - j - 1].rgbtGreen);
            refl(&image[i][j].rgbtBlue, &image[i][width - j - 1].rgbtBlue);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    int R, G, B, counter;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            R = 0;
            G = 0;
            B = 0;
            counter = 0;
            
            for (int x = -1; x < 2; x++)
            {
                if (i + x < 0 || i + x > height - 1)
                {
                    continue;
                }
                for (int y = -1; y < 2; y++)
                {
                    if (j + y < 0 || j + y > width - 1)
                    {
                        continue;
                    }
                    
                    R += image[i + x][j + y].rgbtRed;
                    G += image[i + x][j + y].rgbtGreen;
                    B += image[i + x][j + y].rgbtBlue;
                    counter++;
                }
            }
            
            temp[i][j].rgbtRed = round((float) R / counter);
            temp[i][j].rgbtGreen = round((float) G / counter);
            temp[i][j].rgbtBlue = round((float) B / counter);
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}

//Helper function for Edges, returns corresponding value of Gx kernel
int Gxkernel(int xx, int yy)
{
    switch (yy)
    {
        case -1:
            switch (xx)
            {
                case -1:
                    return -1;
            
                case 0:
                    return -2;
            
                case 1:
                    return -1;
            }
        
        case 0:
            return 0;
        
        case 1:
            switch (xx)
            {
                case -1:
                    return 1;
            
                case 0:
                    return 2;
            
                case 1:
                    return 1;
            }
    }
    
    //the never condition but just to satisfy the programme
    return 1;
}

//Helper function for Edges, returns corresponding value of Gy kernel
int Gykernel(int xx, int yy)
{
    switch (xx)
    {
        case -1:
            switch (yy)
            {
                case -1:
                    return -1;
            
                case 0:
                    return -2;
            
                case 1:
                    return -1;
            }
        
        case 0:
            return 0;
        
        case 1:
            switch (yy)
            {
                case -1:
                    return 1;
                    
                case 0:
                    return 2;
                    
                case 1:
                    return 1;
            }
    }
        
    //the never condition but just to satisfy the programme
    return 1;
}

//Helper function for Edges, uses Sobel filter algorithm and caps at 255
int sobel(int Gx, int Gy)
{
    int n = round(sqrt((float) pow(Gx, 2) + pow(Gy, 2)));
    if (n > 255)
    {
        n = 255;
    }
    return n;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    int GxR, GyR, GxG, GyG, GxB, GyB;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //reset all values to 0
            GxR = 0;
            GyR = 0;
            GxG = 0;
            GyG = 0;
            GxB = 0;
            GyB = 0;
            for (int x = -1; x < 2; x++)
            {
                //if out of image, skip over so value is technically equal to 00
                if (i + x < 0 || i + x > height - 1)
                {
                    continue;
                }
                for (int y = -1; y < 2; y++)
                {
                    //if out of image, skip over so value is technically equal to 00
                    if (j + y < 0 || j + y > width - 1)
                    {
                        continue;
                    }
                    
                    //the calculations
                    GxR += Gxkernel(x, y) * image[i + x][j + y].rgbtRed;
                    GyR += Gykernel(x, y) * image[i + x][j + y].rgbtRed;
                    
                    GxG += Gxkernel(x, y) * image[i + x][j + y].rgbtGreen;
                    GyG += Gykernel(x, y) * image[i + x][j + y].rgbtGreen;
                    
                    GxB += Gxkernel(x, y) * image[i + x][j + y].rgbtBlue;
                    GyB += Gykernel(x, y) * image[i + x][j + y].rgbtBlue;
                }
            }
            
            //store in temporary storage first so it does not alter values for other pixels
            temp[i][j].rgbtRed = sobel(GxR, GyR);
            temp[i][j].rgbtGreen = sobel(GxG, GyG);
            temp[i][j].rgbtBlue = sobel(GxB, GyB);
        }
    }
    
    //copy from temporary storage into actual image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}
