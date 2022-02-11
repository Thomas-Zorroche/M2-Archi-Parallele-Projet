#pragma once

#include "utils.hpp"

int partition(uchar arr[], int l, int r);
uchar kthSmallest(uchar arr[], int l, int r, int k);
int partition(uchar a[], int left, int right, int pIndex);
uchar quickselect(uchar nums[], int left, int right, int k);
void bubbleSort(uchar arr[], int n);
uchar getMedianValue(uchar kernel[], int n);
void medianFilter(Image* image);
void medianFilter_OPTI_1(Image* imag);
