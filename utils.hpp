#pragma once

#include <opencv/cv.h>

typedef struct Image {
    uchar* data;    // Données de l'image
    uint height;     // Hauteur de l'image
    uint width;      // Largeur de l'image
    int channels;   // Nombre de cannaux
    int step;       // Distance entre les deux premiers pixels de lignes successives
    int step_gray;
} Image;

typedef struct Coord {
    int x;
    int y;
} Coord, Point;

typedef struct Kernel {
    float* data;
    uint size;
} Kernel;

void getGrayScaleImage(const Image* imageIN, Image* imageOut);
void applyFilter(const Image* imageIn, Image* imageOut, const Kernel* kernel);
float conv2d(const Image* image, const Coord& pixel, const Kernel* kernel);