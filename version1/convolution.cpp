#include "convolution.hpp"

//  Convolution sur l'image entière
void applyConv2D(const Image* dest, const Kernel* kernel) {
    // Copie temporaire des données de l'image
    Image* copy = copyImage(dest);
    const int step = dest->width * dest->channels * sizeof(uchar);

    for (uint i = 0; i < dest->height; ++i) {
        for (uint j = 0; j < dest->width; ++j) {
            const float convResult = conv2D(copy, Coord((float)j, (float)i), kernel);
            dest->data[i * step + j] = (uchar) clampfs(convResult);
        }
    }

    // On supprime la copie temporaire de l'image
    freeImage(copy);
}

//  Convolution pour 1 pixel
float conv2D(Image* image, const Coord pixel, const Kernel* kernel) {
    const int step = image->width * image->channels * sizeof(uchar);
    float sum = 0.0f;

    for (uint i = 0; i < kernel->height; ++i) {
        for (uint j = 0; j < kernel->width; ++j) {
            uint dX = pixel.x + i - kernel->width / 2;
            uint dY = pixel.y + j - kernel->height / 2;

            const int idKer	    = j * kernel->width + i;
            const int idPixel	= dY * step + dX;
            sum += image->getDataAtPixel(idPixel) * kernel->data[idKer];
        }
    }

    return sum;
}