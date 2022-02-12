#pragma once

#include "../utils/utils.hpp"
#include "convolution.hpp"

void sobel(Image* dest, const float threshold);
void normGradient(const Image* dest, const float** gradients, const uint nbGradients, const float threshold);