#include "sobel.hpp"

void sobel(Image* image) {
    Image* G0 = copyImage(image);
    Image* G90 = copyImage(image);

    // On applique les filtres
    sobelX(G0, &sobel0);
    sobelY(G90, &sobel90);

    // Norme du gradient
    const Image* gradients[2] = {G0, G90};
    normGradient(image, gradients);

    freeImage(G0);
    freeImage(G90);
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