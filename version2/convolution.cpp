#include "convolution.hpp"

// Applique le filtre de sobel X sur l'image
void sobelX(Image* image, float* Gx, const Kernel* kernel) {
    int pixelX, pixelY, idPixel;
    float convResult;

    for (uint i = 0; i < image->height * image->width; ++i) {
        pixelX = i % image->width;
        pixelY = i / image->width;
        idPixel = pixelY * image->width + pixelX;
        convResult = 0.0f;

        convResult += kernel->data[0] * image->getDataAtPixel(idPixel - image->width - 1);
        convResult += kernel->data[2] * image->getDataAtPixel(idPixel - image->width + 1);

        convResult += kernel->data[3] * image->getDataAtPixel(idPixel - 1);
        convResult += kernel->data[5] * image->getDataAtPixel(idPixel + 1);

        convResult += kernel->data[6] * image->getDataAtPixel(idPixel + image->width - 1);
        convResult += kernel->data[8] * image->getDataAtPixel(idPixel + image->width + 1);

        Gx[idPixel] = convResult;
    }
}

// Applique le filtre de sobel Y sur l'image
void sobelY(Image* image, float* Gy, const Kernel* kernel) {
    int pixelX, pixelY, idPixel;
    float convResult;

    for (uint i = 0; i < image->height * image->width; ++i) {
        pixelX = i % image->width;
        pixelY = i / image->width;
        idPixel = pixelY * image->width + pixelX;
        convResult = 0.0f;

        convResult += kernel->data[0] * image->getDataAtPixel(idPixel - image->width - 1);
        convResult += kernel->data[1] * image->getDataAtPixel(idPixel - image->width);
        convResult += kernel->data[2] * image->getDataAtPixel(idPixel - image->width + 1);

        convResult += kernel->data[6] * image->getDataAtPixel(idPixel + image->width - 1);
        convResult += kernel->data[7] * image->getDataAtPixel(idPixel + image->width);
        convResult += kernel->data[8] * image->getDataAtPixel(idPixel + image->width + 1);

        Gy[idPixel] = convResult;
    }
}