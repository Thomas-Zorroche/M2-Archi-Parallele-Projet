#pragma once

#include "../utils/utils.hpp"
#include "convolution.hpp"

void sobel(Image* image);
void normGradient(const Image* dest, const Image** gradients);