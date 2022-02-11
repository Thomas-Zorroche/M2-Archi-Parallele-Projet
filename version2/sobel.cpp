#include "sobel.hpp"

void sobel(Image* image) {
    float* Gx = (float*) malloc(sizeof(float) * image->width * image->height);
    float* Gy = (float*) malloc(sizeof(float) * image->width * image->height);

    // On applique les filtres
    sobelX(image, Gx, &sobel0);
    sobelY(image, Gy, &sobel90);

    // Norme du gradient
    const float* gradients[2] = {Gx, Gy};
    normGradient(image, gradients, 80);

    free(Gx);
    free(Gy);
}

void normGradient(const Image* dest, const float** gradients, const float threshold) {
    const int chs = dest->channels;
    const int step = dest->width * chs * sizeof(uchar);
    float sumAbsGradients;
    int idPixel;

    for(uint i = 0; i < dest->height; ++i) {
        for(uint j = 0; j < dest->width; ++j) {
            idPixel = i * step + j;

            sumAbsGradients = std::abs(gradients[0][idPixel]);
            sumAbsGradients += std::abs(gradients[1][idPixel]);
            
            dest->data[idPixel] = (uchar) clampfs(sumAbsGradients, threshold);
        }
    }
}