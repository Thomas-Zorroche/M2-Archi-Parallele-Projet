#include "median.hpp"

void medianFilter(Image* dest)
{
    Image* copy = copyImage(dest);
    uchar* kernel;
    
    int size = dest->height * dest->width;
    uint size_per_thread = size / NUM_THREADS;
    int id, idPixel;

    #pragma omp parallel shared(size_per_thread, copy, dest) private(id, kernel, idPixel)
    {
        id = omp_get_thread_num(); // Get current thread number
        kernel = (uchar*) malloc(sizeof(uchar) * 3 * 3); // Each thread has its own kernel
        idPixel = (size_per_thread * id);
        
        // Je ne sais pas pourquoi il faut parcourir sur 2 * size_per_thread, mais ça fonctionne ...
        #pragma omp for
        for(uint i = 0; i < 2 * size_per_thread; ++i)
        {
            kernel[0] = copy->getDataAtPixel(idPixel);
            kernel[1] = copy->getDataAtPixel(idPixel + 1);
            kernel[2] = copy->getDataAtPixel(idPixel - 1);

            kernel[3] = copy->getDataAtPixel(idPixel + copy->width);
            kernel[4] = copy->getDataAtPixel(idPixel + copy->width + 1);
            kernel[5] = copy->getDataAtPixel(idPixel + copy->width - 1);

            kernel[6] = copy->getDataAtPixel(idPixel - copy->width);
            kernel[7] = copy->getDataAtPixel(idPixel - copy->width + 1);
            kernel[8] = copy->getDataAtPixel(idPixel - copy->width - 1);
            
            dest->data[idPixel] = opt_med9(kernel);
            idPixel++;
        }

        free(kernel);
    }

    freeImage(copy);
}