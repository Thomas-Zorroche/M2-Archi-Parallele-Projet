#include "sobel.hpp"

void sobel(Image* image) {
    Image* G0 = copyImage(image);
    Image* G90 = copyImage(image);

    // On applique les filtres
    applyConv2D(G0, &sobel0);
    applyConv2D(G90, &sobel90);

    // Norme du gradient
    const Image* gradients[2] = {G0, G90};
    normGradient(image, gradients, 2);

    freeImage(G0);
    freeImage(G90);
}

void normGradient(const Image* dest, const Image** gradients, const uint nbGradients) {
    const int step = gradients[0]->width * gradients[0]->channels * sizeof(uchar);
    int idPixel = 0;
    float sumSquaredGradients = 0.f;

    for(uint i = 0; i < gradients[0]->height; ++i) {
        for(uint j = 0; j < gradients[0]->width; ++j) {
            idPixel = i * step + j;

            // Calcul du carré du gradient pour le pixel
            sumSquaredGradients = 0.f;
            for(uint g = 0; g < nbGradients; ++g) {
                sumSquaredGradients += pow(gradients[g]->data[idPixel], 2);
            }
            
            dest->data[idPixel] = (uchar) clampf(sqrt(sumSquaredGradients));
        }
    }
}