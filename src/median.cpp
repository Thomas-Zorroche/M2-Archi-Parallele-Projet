#include "median.hpp"
#include <omp.h>

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

int ucharComparator ( const void * first, const void * second ) {
    int firstInt = * (const uchar *) first;
    int secondInt = * (const uchar *) second;
    return firstInt - secondInt;
}

#define PIX_SORT(a,b) { if ((a)>(b)) PIX_SWAP((a),(b)); }
#define PIX_SWAP(a,b) { uchar temp=(a);(a)=(b);(b)=temp; }

uchar opt_med9(uchar * p)
{
    PIX_SORT(p[1], p[2]) ; PIX_SORT(p[4], p[5]) ; PIX_SORT(p[7], p[8]) ;
    PIX_SORT(p[0], p[1]) ; PIX_SORT(p[3], p[4]) ; PIX_SORT(p[6], p[7]) ;
    PIX_SORT(p[1], p[2]) ; PIX_SORT(p[4], p[5]) ; PIX_SORT(p[7], p[8]) ;
    PIX_SORT(p[0], p[3]) ; PIX_SORT(p[5], p[8]) ; PIX_SORT(p[4], p[7]) ;
    PIX_SORT(p[3], p[6]) ; PIX_SORT(p[1], p[4]) ; PIX_SORT(p[2], p[5]) ;
    PIX_SORT(p[4], p[7]) ; PIX_SORT(p[4], p[2]) ; PIX_SORT(p[6], p[4]) ;
    PIX_SORT(p[4], p[2]) ; return(p[4]) ;
}

uchar getMedianValue(uchar kernel[], int n)
{
    // Bubble sort
    //bubbleSort(kernel, n);
    //return kernel[(int) (n / 2) + 1];
    
    // Quickselect
    //return kthSmallest(kernel, 0, n-1, (int) (n / 2) + 1);

    //qsort(&kernel[0], n, sizeof(uchar), ucharComparator);
    //return kernel[(int) (n / 2)];

    return opt_med9(kernel);
}

void medianFilter(Image* image)
{
    int kernelSize = 3;
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

void medianFilter_OPTI_1(Image* image)
{
    uchar kernel[(1 + (2 * 3 /* kernelsize */)) * (1 + (2 * 3 /* kernelsize */))];
    int idPixel, pixelX, pixelY = 0;
    
    int size = image->height * image->width;
    uint size_per_thread = size / 2;
    int id;

    #pragma omp parallel shared (size_per_thread, size) private(id)
    {
        id = omp_get_thread_num(); // Get current thread number
        
        #pragma omp for
        for(uint i = 0; i < size_per_thread; ++i)
        {
            pixelX = ((size_per_thread * id) + i) % image->width;
            pixelY = ((size_per_thread * id) + i) / image->width;
            idPixel = pixelY * image->width + pixelX;
            
            kernel[0] = image->getDataAtPixel(idPixel);
            kernel[1] = image->getDataAtPixel(idPixel + 1);
            kernel[2] = image->getDataAtPixel(idPixel - 1);

            kernel[3] = image->getDataAtPixel(idPixel + image->width);
            kernel[4] = image->getDataAtPixel(idPixel + image->width + 1);
            kernel[5] = image->getDataAtPixel(idPixel + image->width - 1);

            kernel[6] = image->getDataAtPixel(idPixel - image->width);
            kernel[7] = image->getDataAtPixel(idPixel - image->width + 1);
            kernel[8] = image->getDataAtPixel(idPixel - image->width - 1);
            
            image->data[i] = getMedianValue(kernel, 9);
        }
    }
    #pragma omp barrier // Wait for all threads to complete
}