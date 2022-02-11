#include "convolution.hpp"

// Applique le filtre de sobel X sur l'image
void sobelX(Image* dest, const Kernel* kernel, const float threshold) {
    // Copie temporaire des données de l'image
    Image* copy = copyImage(dest);
    int pixelX, pixelY, idPixel;
    float convResult;

    for (uint i = 0; i < dest->height * dest->width; ++i) {
        pixelX = i % dest->width;
        pixelY = i / dest->width;
        idPixel = pixelY * dest->width + pixelX;
        convResult = 0.0f;

        convResult += kernel->data[0] * copy->getDataAtPixel(idPixel - copy->width - 1);
        convResult += kernel->data[2] * copy->getDataAtPixel(idPixel - copy->width + 1);

        convResult += kernel->data[3] * copy->getDataAtPixel(idPixel - 1);
        convResult += kernel->data[5] * copy->getDataAtPixel(idPixel + 1);

        convResult += kernel->data[6] * copy->getDataAtPixel(idPixel + copy->width - 1);
        convResult += kernel->data[8] * copy->getDataAtPixel(idPixel + copy->width + 1);

        dest->data[idPixel] = (uchar) clampfs(convResult, threshold);
    }

    // On supprime la copie temporaire de l'image
    freeImage(copy);
}

// Applique le filtre de sobel Y sur l'image
void sobelY(Image* dest, const Kernel* kernel, const float threshold) {
    // Copie temporaire des données de l'image
    Image* copy = copyImage(dest);
    int pixelX, pixelY, idPixel;
    float convResult;

    for (uint i = 0; i < dest->height * dest->width; ++i) {
        pixelX = i % dest->width;
        pixelY = i / dest->width;
        idPixel = pixelY * dest->width + pixelX;
        convResult = 0.0f;

        convResult += kernel->data[0] * copy->getDataAtPixel(idPixel - copy->width - 1);
        convResult += kernel->data[1] * copy->getDataAtPixel(idPixel - copy->width);
        convResult += kernel->data[2] * copy->getDataAtPixel(idPixel - copy->width + 1);

        convResult += kernel->data[6] * copy->getDataAtPixel(idPixel + copy->width - 1);
        convResult += kernel->data[7] * copy->getDataAtPixel(idPixel + copy->width);
        convResult += kernel->data[8] * copy->getDataAtPixel(idPixel + copy->width + 1);

        dest->data[idPixel] = (uchar) clampfs(convResult, threshold);
    }

    // On supprime la copie temporaire de l'image
    freeImage(copy);
}