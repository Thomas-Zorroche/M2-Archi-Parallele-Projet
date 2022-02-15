#pragma once

#include "../utils/utils.hpp"

void sobelX(Image* image, float* Gx, const Kernel* kernel);
void sobelY(Image* image, float* Gy, const Kernel* kernel);