#include "convolution.hpp"

// Applique le filtre de sobel X sur l'image
void sobelX(Image* image, float* Gx, const Kernel* kernel) {
    float convResult;
    uint id, idPixel;

    uint size = image->height * image->width;
    uint size_per_thread = size / NUM_THREADS;

    #pragma omp parallel shared(size, size_per_thread, Gx, kernel) private(id, idPixel, convResult)
    {
        id = omp_get_thread_num(); // Get current thread number
        idPixel = (size_per_thread * id);
        convResult = 0.0f;

        #pragma omp for
        for (uint i = 0; i < size; ++i) {
            convResult  = kernel->data[0] * image->getDataAtPixel(idPixel - image->width - 1);
            convResult += kernel->data[2] * image->getDataAtPixel(idPixel - image->width + 1);

            convResult += kernel->data[3] * image->getDataAtPixel(idPixel - 1);
            convResult += kernel->data[5] * image->getDataAtPixel(idPixel + 1);

            convResult += kernel->data[6] * image->getDataAtPixel(idPixel + image->width - 1);
            convResult += kernel->data[8] * image->getDataAtPixel(idPixel + image->width + 1);

            Gx[idPixel] = convResult;
            idPixel++;
        }
    }
}

// Applique le filtre de sobel Y sur l'image
void sobelY(Image* image, float* Gy, const Kernel* kernel) {
    float convResult;
    uint id, idPixel;

    uint size = image->height * image->width;
    uint size_per_thread = size / NUM_THREADS;

    #pragma omp parallel shared(size, size_per_thread, Gy, kernel) private(id, idPixel, convResult)
    {
        id = omp_get_thread_num(); // Get current thread number
        idPixel = (size_per_thread * id);
        convResult = 0.0f;

        #pragma omp for
        for (uint i = 0; i < size; ++i) {
            convResult = kernel->data[0] * image->getDataAtPixel(idPixel - image->width - 1);
            convResult += kernel->data[1] * image->getDataAtPixel(idPixel - image->width);
            convResult += kernel->data[2] * image->getDataAtPixel(idPixel - image->width + 1);

            convResult += kernel->data[6] * image->getDataAtPixel(idPixel + image->width - 1);
            convResult += kernel->data[7] * image->getDataAtPixel(idPixel + image->width);
            convResult += kernel->data[8] * image->getDataAtPixel(idPixel + image->width + 1);

            Gy[idPixel] = convResult;
            idPixel++;
        }
    }
}