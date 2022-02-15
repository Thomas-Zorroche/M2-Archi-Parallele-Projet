#include "utils.hpp"

 // Conversion RGB en niveau de gris
void getGrayScaleImage(const Image* imageIn, Image* imageOut) {
    uint sizeIn = imageIn->width * imageIn->height * imageIn->channels;
    uint sizeOut = imageOut->width * imageOut->height;
    uint size_per_thread_in = sizeIn / NUM_THREADS;
    uint size_per_thread_out = sizeOut / NUM_THREADS;
    uint id, idPixelIn, idPixelOut;

    #pragma omp parallel shared(sizeOut, size_per_thread_in, size_per_thread_out, imageIn, imageOut) private(id, idPixelIn, idPixelOut)
    {
        id = omp_get_thread_num();
        idPixelIn = (size_per_thread_in * id);
        idPixelOut = (size_per_thread_out * id);

        #pragma omp for
        for(uint i = 0; i < sizeOut; ++i) {
            imageOut->data[idPixelOut] = 0.114 * imageIn->data[idPixelIn] + 0.587 * imageIn->data[idPixelIn + 1] + 0.299 * imageIn->data[idPixelIn + 2];
            idPixelOut++;
            idPixelIn += 3;
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