#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <opencv/highgui.h>
#include <opencv/cv.h>

#include <assert.h>
#include <time.h>

typedef struct Image {
    uchar* data;    // Données de l'image
    uint height;    // Hauteur de l'image
    uint width;     // Largeur de l'image
    int channels;   // Nombre de cannaux

    uchar getDataAtPixel(int idPixel);
} Image;

typedef struct Coord {
    float x;
    float y;
    float z = 0;
} Coord, Point, float3;

typedef struct Kernel {
    float* data;
    uint width;
    uint height;
} Kernel;

void getGrayScaleImage(const Image* imageIn, Image* imageOut);

void applyConv2dGray(const Image* image, const Kernel* kernel);
void applyConv2dColor(const Image* image, const Kernel* kernel);

float conv2dGray(const Image* dest, const Coord pixel, const Kernel* kernel);
float3 conv2dColor(const Image* dest, const Coord pixel, const Kernel* kernel);

void medianFilter(Image* image, int kernelSize = 1);

Image* copyImage(const Image* image);
void freeImage(Image* image);

float clampf(const float value, const float max = 255, const float min = 0);