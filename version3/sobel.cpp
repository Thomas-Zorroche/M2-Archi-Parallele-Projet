#include "sobel.hpp"

void sobel(Image* image) {
    Image* Gx = copyImage(image);
    Image* Gy = copyImage(image);

    // On applique les filtres
    const float threshold = 60;
    sobelX(Gx, &sobel0, threshold);
    sobelY(Gy, &sobel90, threshold);

    // Norme du gradient
    const Image* gradients[2] = {Gx, Gy};
    normGradient(image, gradients);

    freeImage(Gx);
    freeImage(Gy);
}

void normGradient(const Image* dest, const Image** gradients) {
    const int chs = dest->channels;
    const int step = dest->width * chs * sizeof(uchar);
    float sumAbsGradients;
    int idPixel;

    for(uint i = 0; i < dest->height; ++i) {
        for(uint j = 0; j < dest->width; ++j) {
            idPixel = i * step + j;

            sumAbsGradients = std::abs(gradients[0]->data[idPixel]);
            sumAbsGradients += std::abs(gradients[1]->data[idPixel]);
            
            dest->data[idPixel] = (uchar) clampf(sumAbsGradients);
        }
    }
}