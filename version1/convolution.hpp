#pragma once

#include "../utils/utils.hpp"

void applyConv2D(Image* dest, float* gradient, const Kernel* kernel);
float conv2D(Image* image, const Coord pixel, const Kernel* kernel);