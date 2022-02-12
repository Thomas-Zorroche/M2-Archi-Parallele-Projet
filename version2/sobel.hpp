#pragma once

#include "../utils/utils.hpp"
#include "convolution.hpp"

void sobel(Image* image, const float threshold);
void normGradient(const Image* dest, const float** gradients, const float threshold);