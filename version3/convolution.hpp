#pragma once

#include "../utils/utils.hpp"

void sobelX(Image* dest, const Kernel* kernel, const float threshold);
void sobelY(Image* dest, const Kernel* kernel, const float threshold);