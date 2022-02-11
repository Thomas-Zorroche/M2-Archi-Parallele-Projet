#include "median.hpp"

uchar getMedianValue(uchar kernel[], int n)
{
    return opt_med9(kernel);
}

void medianFilter(Image* dest)
{
    Image* copy = copyImage(dest);
    uchar kernel[(1 + (2 * 3 /* kernelsize */)) * (1 + (2 * 3 /* kernelsize */))];
    int idPixel, pixelX, pixelY = 0;
    
    int size = dest->height * dest->width;
    uint size_per_thread = size / (float)omp_get_max_threads();
    int id;

    #pragma omp parallel shared (size_per_thread, size) private(id)
    {
        id = omp_get_thread_num(); // Get current thread number
        
        #pragma omp for
        for(uint i = 0; i < size_per_thread; ++i)
        {
            pixelX = ((size_per_thread * id) + i) % dest->width;
            pixelY = ((size_per_thread * id) + i) / dest->width;
            idPixel = pixelY * dest->width + pixelX;
            
            kernel[0] = copy->getDataAtPixel(idPixel);
            kernel[1] = copy->getDataAtPixel(idPixel + 1);
            kernel[2] = copy->getDataAtPixel(idPixel - 1);

            kernel[3] = copy->getDataAtPixel(idPixel + copy->width);
            kernel[4] = copy->getDataAtPixel(idPixel + copy->width + 1);
            kernel[5] = copy->getDataAtPixel(idPixel + copy->width - 1);

            kernel[6] = copy->getDataAtPixel(idPixel - copy->width);
            kernel[7] = copy->getDataAtPixel(idPixel - copy->width + 1);
            kernel[8] = copy->getDataAtPixel(idPixel - copy->width - 1);
            
            dest->data[i] = getMedianValue(kernel, 9);
        }
    }
    #pragma omp barrier // Wait for all threads to complete

    freeImage(copy);
}