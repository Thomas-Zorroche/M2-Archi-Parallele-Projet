#include "median.hpp"

uchar getMedianValue(uchar kernel[], int n)
{
    // Bubble sort
    bubbleSort(kernel, n);
    return kernel[(int) (n / 2)];
    
    // Quickselect
    // return kthSmallest(kernel, 0, n-1, (int) (n / 2) + 1);

    // qsort(&kernel[0], n, sizeof(uchar), ucharComparator);
    // return kernel[(int) (n / 2)];
}

void medianFilter(Image* image)
{
    int kernelSize = 1; // 1 --> Correspond à un kernel de 3 * 3
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