#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <opencv/highgui.h>
#include <opencv/cv.h>

#include <assert.h>
#include <time.h>
#include <omp.h>
#include "chrono.hpp"

static const uint NUM_THREADS = omp_get_max_threads();
static const float THRESHOLD = 95.f;

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
    float z;
    Coord(float _x, float _y, float _z = 0) { x = _x; y = _y; z = _z; }
} Coord, Point, float3;

typedef struct Kernel {
    float* data;
    uint width;
    uint height;
} Kernel;

void getGrayScaleImage(const Image* imageIn, Image* imageOut);

Image* copyImage(const Image* image);
void freeImage(Image* image);

float clampf(const float value, const float max = 255, const float min = 0);
float clampfs(const float value, const float threshold = 30, const float max = 255, const float min = 0);

// Filtres de Sobel
// static float dataSobelX[] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
static float dataSobel0[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
static const Kernel sobel0 = {dataSobel0, 3, 3};
// static float dataSobelY[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
static float dataSobel90[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
static const Kernel sobel90 = {dataSobel90, 3, 3};
// static float dataSobel45[] = {-2, -1, 0, -1, 0, 1, 0, 1, 2};
static float dataSobel45[] = {2, 1, 0, 1, 0, -1, 0, -1, -2};
static const Kernel sobel45 = {dataSobel45, 3, 3};
// static float dataSobel135[] = {0, 1, 2, -1, 0, 1, -2, -1, 0};
static float dataSobel135[] = {0, -1, -2, 1, 0, -1, 2, 1, 0};
static const Kernel sobel135 = {dataSobel135, 3, 3};