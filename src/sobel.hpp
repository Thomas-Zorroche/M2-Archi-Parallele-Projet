#pragma once

#include "utils.hpp"

// Filtres
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

// Optimisations
static float dataSobel0A[] = {-1, 0, 1}; // Horizontal
static const Kernel sobel0A = {dataSobel0A, 1, 3};
static float dataSobel0B[] = {1, 2, 1}; // Vertical
static const Kernel sobel0B = {dataSobel0B, 3, 1};

static float dataSobel90A[] = {1, 2, 1}; // Horizontal
static const Kernel sobel90A = {dataSobel90A, 1, 3};
static float dataSobel90B[] = {-1, 0, 1}; // Vertical
static const Kernel sobel90B = {dataSobel90B, 3, 1};

void sobel(Image* image);
void normGradient(const Image* dest, const Image** gradients, const uint nbGradients);
void normGradient_OPTI_1(const Image* dest, const Image** gradients);