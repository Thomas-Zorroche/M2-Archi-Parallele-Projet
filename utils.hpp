#pragma once

#include <opencv/cv.h>

typedef struct Image {
    uchar* data;    // Données de l'image
    uint height;    // Hauteur de l'image
    uint width;     // Largeur de l'image
    int channels;   // Nombre de cannaux
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

void getGrayScaleImage(const Image* imageIN, Image* imageOut);
void applyFilterGray(const Image* image, const Kernel* kernel);
void applyFilterColor(const Image* image, const Kernel* kernel);
float conv2dGray(const Image* image, const Coord& pixel, const Kernel* kernel);
float3 conv2dColor(const Image* image, const Coord& pixel, const Kernel* kernel);
Image* copyImage(const Image* image);
void freeImage(Image* image);