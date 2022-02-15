#include "convolution.hpp"

// Applique le filtre de sobel X sur l'image
void sobelX(Image* image, float* Gx, const Kernel* kernel) {
    float convResult;
    uint idPixel;

    #pragma omp parallel shared(Gx, image, kernel) private(idPixel, convResult)
    {
        convResult = 0.0f;

        #pragma omp for
        for (uint i = 1; i < image->height - 1; ++i) {
            for(uint j = 1; j < image->width - 1; ++j) {
                idPixel = i * image->width + j;

                convResult  = kernel->data[0] * image->data[idPixel - image->width - 1];
                convResult += kernel->data[2] * image->data[idPixel - image->width + 1];

                convResult += kernel->data[3] * image->data[idPixel - 1];
                convResult += kernel->data[5] * image->data[idPixel + 1];

                convResult += kernel->data[6] * image->data[idPixel + image->width - 1];
                convResult += kernel->data[8] * image->data[idPixel + image->width + 1];

                Gx[idPixel] = convResult;
            }
        }
    }
}

// Applique le filtre de sobel Y sur l'image
void sobelY(Image* image, float* Gy, const Kernel* kernel) {
    float convResult;
    uint idPixel;

    #pragma omp parallel shared(Gy, image, kernel) private(idPixel, convResult)
    {
        convResult = 0.0f;

        #pragma omp for
        for (uint i = 1; i < image->height - 1; ++i) {
            for(uint j = 1; j < image->width - 1; ++j) {
                idPixel = i * image->width + j;

                convResult  = kernel->data[0] * image->data[idPixel - image->width - 1];
                convResult += kernel->data[1] * image->data[idPixel - image->width];
                convResult += kernel->data[2] * image->data[idPixel - image->width + 1];

                convResult += kernel->data[6] * image->data[idPixel + image->width - 1];
                convResult += kernel->data[7] * image->data[idPixel + image->width];
                convResult += kernel->data[8] * image->data[idPixel + image->width + 1];

                Gy[idPixel] = convResult;
            }
        }
    }
}