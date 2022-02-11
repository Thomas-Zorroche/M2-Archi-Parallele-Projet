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

uchar Image::getDataAtPixel(int idPixel) {
    static const int numPixels = width * height;
    return (idPixel >= numPixels || idPixel < 0) ? 0 : data[idPixel];
}

// Clamp value using min / max values
float clampf(const float value, const float max, const float min) {
    return (value <= min ? min : (value >= max ? max : value));
}

// Clamp value using min / max values and a threshold
float clampfs(const float value, const float threshold, const float max, const float min) {
    return (value >= threshold) ? max : min;
}