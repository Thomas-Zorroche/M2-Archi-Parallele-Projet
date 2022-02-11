#pragma once

#include "utils.hpp"

void swap(uchar *xp, uchar *yp);

int partition(uchar arr[], int l, int r);

uchar quickselect(uchar nums[], int left, int right, int k);

uchar kthSmallest(uchar arr[], int l, int r, int k);

void bubbleSort(uchar arr[], int n);

int ucharComparator(const void * first, const void * second);

uchar opt_med9(uchar * p);