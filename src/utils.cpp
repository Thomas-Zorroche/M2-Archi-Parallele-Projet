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
            // const float convResult = conv2dGray(copy, Coord((float)j, (float)i), kernel);
            const float convResult = conv2dGray_OPTI_1(copy, Coord((float)j, (float)i), kernel);
            dest->data[i * step + j] = (uchar) clampfs(convResult);
        }
    }

    // On supprime la copie temporaire de l'image
    freeImage(copy);
}

void applyConv2dGray_OPTI_1(Image* dest, const Kernel* kernel) {
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

        convResult += kernel->data[3] * copy->getDataAtPixel(idPixel - 1);
        convResult += kernel->data[4] * copy->getDataAtPixel(idPixel);
        convResult += kernel->data[5] * copy->getDataAtPixel(idPixel + 1);

        convResult += kernel->data[6] * copy->getDataAtPixel(idPixel + copy->width - 1);
        convResult += kernel->data[7] * copy->getDataAtPixel(idPixel + copy->width);
        convResult += kernel->data[8] * copy->getDataAtPixel(idPixel + copy->width + 1);

        dest->data[idPixel] = (uchar) clampfs(convResult);
    }

    // On supprime la copie temporaire de l'image
    freeImage(copy);
}

void applyConv2dGray_OPTI_2(Image* dest, const Kernel* kernelA, const Kernel* kernelB) {
    // Copie temporaire des données de l'image
    float* copy = allocateFloatDataImage(dest);
    int pixelX, pixelY, idPixel;
    float convResult;

    // Horizontal kernel
    for (uint i = 0; i < dest->height * dest->width; ++i) {
        pixelX = i % dest->width;
        pixelY = i / dest->width;
        idPixel = pixelY * dest->width + pixelX;
        convResult = 0;

        convResult += kernelA->data[0] * dest->getDataAtPixel(idPixel - 1);
        convResult += kernelA->data[1] * dest->getDataAtPixel(idPixel);
        convResult += kernelA->data[2] * dest->getDataAtPixel(idPixel + 1);

        copy[idPixel] = convResult; // Ne pas clamp ici !
    }

    // Vertical kernel
    for (uint i = 0; i < dest->height * dest->width; ++i) {
        pixelX = i % dest->width;
        pixelY = i / dest->width;
        idPixel = pixelY * dest->width + pixelX;
        convResult = 0;

        convResult += kernelB->data[0] * getDataAtPixel(copy, idPixel - dest->width, dest->width, dest->height);
        convResult += kernelB->data[1] * getDataAtPixel(copy, idPixel, dest->width, dest->height);
        convResult += kernelB->data[2] * getDataAtPixel(copy, idPixel + dest->width, dest->width, dest->height);

        dest->data[idPixel] = (uchar) clampfs(convResult);
    }

    // On supprime la copie temporaire de l'image
    free(copy);
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

float conv2dGray_OPTI_1(Image* image, const Coord pixel, const Kernel* kernel) {
    float sum = 0;
    uint idPixel = pixel.y * image->width + pixel.x;


    sum += kernel->data[3] * image->getDataAtPixel(idPixel);
    sum += kernel->data[4] * image->getDataAtPixel(idPixel + 1);
    sum += kernel->data[5] * image->getDataAtPixel(idPixel - 1);

    sum += kernel->data[0] * image->getDataAtPixel(idPixel + image->width);
    sum += kernel->data[1] * image->getDataAtPixel(idPixel + image->width + 1);
    sum += kernel->data[2] * image->getDataAtPixel(idPixel + image->width - 1);

    sum += kernel->data[6] * image->getDataAtPixel(idPixel - image->width);
    sum += kernel->data[7] * image->getDataAtPixel(idPixel - image->width + 1);
    sum += kernel->data[8] * image->getDataAtPixel(idPixel - image->width - 1);

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

float* allocateFloatDataImage(const Image* image) {
    float* copy = nullptr;
    const size_t length = image->width * image->height * image->channels;
    const size_t bytes = sizeof(float) * length;
    copy = (float*) malloc(bytes);
    return copy;
}

float getDataAtPixel(const float* data, int idPixel, const int width, const int height) {
    static const int numPixels = width * height;
    return (idPixel >= numPixels || idPixel < 0) ? 0 : data[idPixel];
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

float clampfs(const float value, const float threshold, const float max, const float min) {
    return (value >= threshold) ? max : min;
}
