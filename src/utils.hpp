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

// Filtres
static float dataSobelX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
static const Kernel sobelX = {dataSobelX, 3, 3};
static float dataSobelY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
static const Kernel sobelY = {dataSobelY, 3, 3};

void getGrayScaleImage(const Image* imageIn, Image* imageOut);

void applyFilterGray(const Image* image, const Kernel* kernel);
void applyFilterColor(const Image* image, const Kernel* kernel);

float conv2dGray(const Image* dest, const Coord pixel, const Kernel* kernel);
float3 conv2dColor(const Image* dest, const Coord pixel, const Kernel* kernel);

void sobel(Image* image);
void normGradient(const Image* dest, const Image* Gx, const Image* Gy);

void medianFilter(Image* image, int kernelSize = 1);

Image* copyImage(const Image* image);
void freeImage(Image* image);

float clampf(const float value, const float max = 255, const float min = 0);