#pragma once

#include "../utils/utils.hpp"
#include "../utils/sort.hpp"
#include <omp.h>

uchar getMedianValue(uchar kernel[], int n);

void medianFilter(Image* dest);
