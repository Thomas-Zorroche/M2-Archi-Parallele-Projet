#pragma once

#include "utils.hpp"

// Filtres
// static float dataSobelX[] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
static float dataSobelX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
static const Kernel sobelX = {dataSobelX, 3, 3};
// static float dataSobelY[] = {1, 2, 1, 0, 0, 0, -1, -2, -1};
static float dataSobelY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
static const Kernel sobelY = {dataSobelY, 3, 3};

void sobel(Image* image);
void normGradient(const Image* dest, const Image* Gx, const Image* Gy);