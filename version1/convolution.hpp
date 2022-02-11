#pragma once

#include "../utils/utils.hpp"

void applyConv2D(const Image* dest, const Kernel* kernel);
float conv2D(Image* image, const Coord pixel, const Kernel* kernel);