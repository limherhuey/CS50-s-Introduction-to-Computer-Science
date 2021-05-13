#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float gray;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            gray = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = gray;
            image[i][j].rgbtGreen = gray;
            image[i][j].rgbtRed = gray;
        }
    }
    return;
}

//helper function for sepia to draw borders
int max(int sepiaColour)
{
    if (sepiaColour > 255)
    {
        sepiaColour = 255;
    }
    return sepiaColour;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int sepiaR;
    int sepiaG;
    int sepiaB;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sepiaR = max(round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue));
            sepiaG = max(round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue));
            sepiaB = max(round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue));
            
            image[i][j].rgbtRed = sepiaR;
            image[i][j].rgbtGreen = sepiaG;
            image[i][j].rgbtBlue = sepiaB;
        }
    }
    return;
}

//helper function for reflect, to reflect left bit and corresponding right bit
void reflection(BYTE *left, BYTE *right)
{
    BYTE pixelholder = *left;
    *left = *right;
    *right = pixelholder;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, w = width / 2; j < w; j++)
        {
            reflection(&image[i][j].rgbtRed, &image[i][width - 1 - j].rgbtRed);
            reflection(&image[i][j].rgbtGreen, &image[i][width - 1 - j].rgbtGreen);
            reflection(&image[i][j].rgbtBlue, &image[i][width - 1 - j].rgbtBlue);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //an array to store values temporarily so subsequent calculations will not be affected
    RGBTRIPLE store[height][width];
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int aveRed = 0;
            int aveGreen = 0;
            int aveBlue = 0;
            int nSquares = 0;
            
            //to determine all the bits around a single bit
            for (int a = -1; a < 2; a++)
            {
                //skip bits out of frame
                if (i + a < 0 || i + a > height - 1)
                {
                    continue;
                }
                for (int b = -1; b < 2; b++)
                {
                    if (j + b < 0 || j + b > width - 1)
                    {
                        continue;
                    }
                    aveRed += image[i + a][j + b].rgbtRed;
                    aveGreen += image[i + a][j + b].rgbtGreen;
                    aveBlue += image[i + a][j + b].rgbtBlue;
                    nSquares++;
                }
            }
            
            //copy average values into store
            store[i][j].rgbtRed = round((float) aveRed / nSquares);
            store[i][j].rgbtGreen = round((float) aveGreen / nSquares);
            store[i][j].rgbtBlue = round((float) aveBlue / nSquares);
        }
    }
    
    //copy values from store to actual bits
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = store[i][j].rgbtRed;
            image[i][j].rgbtGreen = store[i][j].rgbtGreen;
            image[i][j].rgbtBlue = store[i][j].rgbtBlue;
        }
    }
    
    return;
}