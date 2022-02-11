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

uchar getMedianValue(uchar kernel[], int n)
{
    // Bubble sort
    //bubbleSort(kernel, n);
    //return kernel[(int) (n / 2) + 1];
    
    // Quickselect
    return kthSmallest(kernel, 0, n-1, (int) (n / 2) + 1);
}

void medianFilter(Image* image, Chrono& chrono, int kernelSize)
{
    const int step = image->width;

    uchar kernel[(1 + (2 * kernelSize)) * (1 + (2 * kernelSize))];
    
    int idKernel, idNeighbour, idPixel = 0;
    for(uint i = 0; i < image->height; ++i) 
    {
        for(uint j = 0; j < image->width; ++j) 
        {
            idPixel = i * step + j;
            idKernel = 0;
            
            for (int kx = -kernelSize; kx <= kernelSize; kx++)
            {
                for (int ky = -kernelSize; ky <= kernelSize; ky++)
                {
                    idNeighbour = idPixel + ky + (kx * image->width);
                    kernel[idKernel++] = image->getDataAtPixel(idNeighbour);
                } 
            }
            image->data[idPixel] = getMedianValue(kernel, idKernel);
        }
    }
}

void medianFilter_OPTI_1(Image* image, int kernelSize)
{
    const int step = image->width;

    uchar kernel[(1 + (2 * kernelSize)) * (1 + (2 * kernelSize))];
    
    int idKernel, idNeighbour, pixelX, pixelY, dX, dY = 0;
    
    static const uint halfWidth = kernelSize / 2;
    
    for(uint i = 0; i < image->height * image->width; ++i) 
    {
        pixelX = i % image->width;
        pixelY = i / image->width;

        idKernel = 0;
        #pragma omp parallel for
        for (int idKer = 0; idKer < kernelSize * kernelSize; idKer++)
        {
            dX = pixelX + (idKer % kernelSize) - halfWidth;
            dY = pixelY + (idKer / kernelSize) - halfWidth;
            idNeighbour = dY * step + dX;
            kernel[idKernel++] = image->getDataAtPixel(idNeighbour);
        }
        image->data[i] = getMedianValue(kernel, idKernel);
    }
}