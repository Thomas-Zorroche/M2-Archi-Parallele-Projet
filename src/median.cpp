#include "median.hpp"

#define SWAP(x, y) { uchar temp = x; x = y; y = temp; }

void swap(uchar *xp, uchar *yp)
{
    uchar temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// QUICKSELECT ALGORITHM
int partition(uchar arr[], int l, int r)
{
    int x = arr[r], i = l;
    for (int j = l; j <= r - 1; j++) {
        if (arr[j] <= x) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    swap(&arr[i], &arr[r]);
    return i;
}

uchar kthSmallest(uchar arr[], int l, int r, int k)
{
    // If k is smaller than number of
    // elements in array
    if (k > 0 && k <= r - l + 1) {
 
        // Partition the array around last
        // element and get position of pivot
        // element in sorted array
        int index = partition(arr, l, r);
 
        // If position is same as k
        if (index - l == k - 1)
            return arr[index];
 
        // If position is more, recur
        // for left subarray
        if (index - l > k - 1)
            return kthSmallest(arr, l, index - 1, k);
 
        // Else recur for right subarray
        return kthSmallest(arr, index + 1, r,
                            k - index + l - 1);
    }
 
    // If k is more than number of
    // elements in arrayquickselect
    return 255;
}

// QUICKSELECT 2
int partition(uchar a[], int left, int right, int pIndex)
{
    // pick `pIndex` as a pivot from the array
    int pivot = a[pIndex];
 
    // Move pivot to end
    SWAP(a[pIndex], a[right]);
 
    // elements less than the pivot will be pushed to the left of `pIndex`;
    // elements more than the pivot will be pushed to the right of `pIndex`;
    // equal elements can go either way
    pIndex = left;
 
    // each time we find an element less than or equal to the pivot, `pIndex`
    // is incremented, and that element would be placed before the pivot.
    for (int i = left; i < right; i++)
    {
        if (a[i] <= pivot)
        {
            SWAP(a[i], a[pIndex]);
            pIndex++;
        }
    }
 
    // move pivot to its final place
    SWAP(a[pIndex], a[right]);
 
    // return `pIndex` (index of the pivot element)
    return pIndex;
}

uchar quickselect(uchar nums[], int left, int right, int k)
{
    // If the array contains only one element, return that element
    if (left == right) {
        return nums[left];
    }
 
    // select `pIndex` between left and right
    int pIndex = left + rand() % (right - left + 1);
 
    pIndex = partition(nums, left, right, pIndex);
 
    // The pivot is in its final sorted position
    if (k == pIndex) {
        return nums[k];
    }
 
    // if `k` is less than the pivot index
    else if (k < pIndex) {
        return quickselect(nums, left, pIndex - 1, k);
    }
 
    // if `k` is more than the pivot index
    else {
        return quickselect(nums, pIndex + 1, right, k);
    }
}

// BUBBLE SORT
void bubbleSort(uchar arr[], int n)
{
   int i, j;
   for (i = 0; i < n-1; i++)      
  {
       // Last i elements are already in place   
       for (j = 0; j < n-i-1; j++) 
       {
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
       }
  }
}

uchar getMedianValue(uchar kernel[], int n, Chrono& chrono)
{
    // Bubble sort
    //bubbleSort(kernel, n);
    //return kernel[(int) (n / 2) + 1];
    
    // Quickselect #1
    //printf("%d", (int)n/2);
    // return kthSmallest(kernel, 0, n-1, (int) (n / 2) + 1);

    // Quickselect #2
    return quickselect(kernel, 0, n - 1, (int) (n / 2) + 1);
}

void medianFilter(Image* image, Chrono& chrono, int kernelSize)
{
    const int chs = image->channels; // 1
    const int step = image->width * chs /* * sizeof(uchar)*/;

    uchar kernel[(1 + (2 * kernelSize)) * (1 + (2 * kernelSize))];
    
    // #pragma omp parallel for collapse(2)
    for(uint i = 0; i < image->height; ++i) 
    {
        for(uint j = 0; j < image->width; ++j) 
        {
            const int idPixel = i * step + j * chs;
            int idKernel = 0;
            
            for (int kx = -kernelSize; kx <= kernelSize; kx++)
            {
                for (int ky = -kernelSize; ky <= kernelSize; ky++)
                {
                    int idNeighbour = idPixel + (ky * chs) + (kx * image->width);
                    kernel[idKernel] = image->getDataAtPixel(idNeighbour);
                    idKernel++;
                } 
            }
            image->data[idPixel] = getMedianValue(kernel, idKernel, chrono);
        }
    }
    
}