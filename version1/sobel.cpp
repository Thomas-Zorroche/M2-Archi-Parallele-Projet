#include "sobel.hpp"

void sobel(Image* dest, const float threshold) {
    float* Gx = (float*) malloc(sizeof(float) * dest->width * dest->height);
    float* Gy = (float*) malloc(sizeof(float) * dest->width * dest->height);

    // On applique les filtres
    applyConv2D(dest, Gx, &sobel0);
    applyConv2D(dest, Gy, &sobel90);

    // Norme du gradient
    const float* gradients[2] = {Gx, Gy};
    normGradient(dest, gradients, 2, threshold);

    free(Gx);
    free(Gy);
}

void normGradient(const Image* dest, const float** gradients, const uint nbGradients, const float threshold) {
    const int step = dest->width * dest->channels * sizeof(uchar);
    float sumSquaredGradients = 0.f;
    int idPixel = 0;

    for(uint i = 0; i < dest->height; ++i) {
        for(uint j = 0; j < dest->width; ++j) {
            idPixel = i * step + j;

            // Calcul du carré du gradient pour le pixel
            sumSquaredGradients = 0.f;
            for(uint g = 0; g < nbGradients; ++g) {
                sumSquaredGradients += pow(gradients[g][idPixel], 2);
            }
            
            dest->data[idPixel] = (uchar) clampfs(sqrt(sumSquaredGradients), threshold);
        }
    }
}