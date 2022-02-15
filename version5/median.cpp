#include "median.hpp"

void medianFilter(Image* dest)
{
    Image* copy = copyImage(dest);
    uchar* kernel;
    int idPixel;

    #pragma omp parallel shared(copy, dest) private(kernel, idPixel)
    {
        kernel = (uchar*) malloc(sizeof(uchar) * 3 * 3); // Each thread has its own kernel
        
        #pragma omp for
        for(uint i = 1; i < dest->height; ++i)
        {
            for(uint j = 1; j < dest->width - 1; ++j) {
                idPixel = i * dest->width + j;

                kernel[0] = copy->data[idPixel];
                kernel[1] = copy->data[idPixel + 1];
                kernel[2] = copy->data[idPixel - 1];

                kernel[3] = copy->data[idPixel + copy->width];
                kernel[4] = copy->data[idPixel + copy->width + 1];
                kernel[5] = copy->data[idPixel + copy->width - 1];

                kernel[6] = copy->data[idPixel - copy->width];
                kernel[7] = copy->data[idPixel - copy->width + 1];
                kernel[8] = copy->data[idPixel - copy->width - 1];
                
                dest->data[idPixel] = opt_med9(kernel);
            }
        }

        free(kernel);
    }

    freeImage(copy);
}