#include "utils.hpp"

 // Conversion RGB en niveau de gris
void getGrayScaleImage(const Image* imageIn, Image* imageOut) {
    const int chs = imageIn->channels;
    const int stepImageIn = imageIn->width * chs * sizeof(uchar);
    const int stepImageOut = imageOut->width * imageOut->channels * sizeof(uchar);

    for(uint i = 0; i < imageIn->height; ++i) {
        for(uint j = 0; j < imageIn->width; ++j) {
            const int idPixel = i * stepImageIn + j * chs;
            imageOut->data[i * stepImageOut + j] = 0.114 * imageIn->data[idPixel + 0] + 0.587 * imageIn->data[idPixel + 1] + 0.299 * imageIn->data[idPixel + 2];
        }
    }
}

void applyConv2dGray(const Image* dest, const Kernel* kernel) {
    // Copie temporaire des données de l'image
    Image* copy = copyImage(dest);
    const int step = dest->width * dest->channels * sizeof(uchar);

    for (uint i = 0; i < dest->height; ++i) {
        for (uint j = 0; j < dest->width; ++j) {
            const float convResult = conv2dGray(copy, {(float)j, (float)i}, kernel);
            // const float convResult = conv2dGray_OPTI_1(copy, {(float)j, (float)i}, kernel);
            dest->data[i * step + j] = (uchar) clampf(convResult);
        }
    }

    // On supprime la copie temporaire de l'image
    freeImage(copy);
}

float conv2dGray(const Image* image, const Coord pixel, const Kernel* kernel) {
    const uint kernelWidth = kernel->width;
    const uint kernelHeight = kernel->height;
    const uint width = image->width;
    const uint height = image->height;
    const int chs = image->channels;
    const int step = image->width * chs * sizeof(uchar);

    float sum = 0.0f;

    for (uint i = 0; i < kernelHeight; ++i) {
        for (uint j = 0; j < kernelWidth; ++j) {
            uint dX = pixel.x + i - kernelWidth / 2;
            uint dY = pixel.y + j - kernelHeight / 2;

            // Handle borders
            if (dX < 0)
                dX = 0;

            if (dX >= width)
                dX = width - 1;

            if (dY < 0) 
                dY = 0;

            if (dY >= height)
                dY = height - 1;

            const int idKer		= j * kernelWidth + i;
            const int idPixel	= dY * step + dX;
            sum += image->data[idPixel] * kernel->data[idKer];
        }
    }

    return sum;
}

float conv2dGray_OPTI_1(const Image* image, const Coord pixel, const Kernel* kernel) {
    const uint kernelWidth = kernel->width;
    const uint kernelHeight = kernel->height;
    const int width = image->width;
    const int height = image->height;
    const uint chs = image->channels;
    const uint step = image->width * chs * sizeof(uchar);

    float sum = 0.0f;

    const uint halfWidth = kernelWidth / 2;
    const uint halfHeight = kernelHeight / 2;

    for (uint idKer = 0; idKer < kernelHeight * kernelWidth; ++idKer) {
        int dX = pixel.x + (idKer % kernelWidth) - halfWidth;
        int dY = pixel.y + (idKer / kernelHeight) - halfHeight;

        // Handle borders
        if (dX < 0)
            dX = 0;

        if (dX >= width)
            dX = width - 1;

        if (dY < 0) 
            dY = 0;

        if (dY >= height)
            dY = height - 1;

        const uint idPixel = dY * step + dX;
        sum += image->data[idPixel] * kernel->data[idKer];
    }

    return sum;
}

// Copie l'image d'entrée et retourne un pointeur vers la copie créée
Image* copyImage(const Image* image) {
    // Allocation mémoire pour la structure
    Image* copy = (Image*) malloc(sizeof(Image));

    // Copie des données dans la nouvelle structure
    copy->channels = image->channels;
    copy->height = image->height;
    copy->width = image->width;

    // Copie des données de l'image
    const size_t length = image->width * image->height * image->channels;
    const size_t bytes = sizeof(uchar) * length;
    copy->data = (uchar*) malloc(bytes);
    memcpy(copy->data, image->data, bytes);

    return copy;
}

void freeImage(Image* image) {
    free(image->data);
    free(image);
}

uchar Image::getDataAtPixel(int idPixel)
{
    static const int numPixels = width * height;
    return (idPixel >= numPixels || idPixel < 0) ? 0 : data[idPixel];
}

float clampf(const float value, const float max, const float min) {
    return (value <= min ? min : (value >= max ? max : value));
}
