#include "sobel.hpp"

void sobel(Image* image) {
    Image* G0 = copyImage(image);
    Image* G90 = copyImage(image);
    Image* G45 = copyImage(image);
    Image* G135 = copyImage(image);

    // On applique les filtres
    if(G0->channels == 1) {
        applyConv2dGray(G0, &sobel0);
        applyConv2dGray(G90, &sobel90);
        applyConv2dGray(G45, &sobel45);
        applyConv2dGray(G135, &sobel135);
    } else {
        applyConv2dGray(G0, &sobel0);
        applyConv2dGray(G90, &sobel90);
        // applyConv2dGray(G45, &sobel45);
        // applyConv2dGray(G135, &sobel135);
    }

    // Norme du gradient
    const Image* gradients[4] = {G0, G90, G45, G135};
    normGradient(image, gradients, 4);

    freeImage(G0);
    freeImage(G90);
    freeImage(G45);
    freeImage(G135);
}

void normGradient(const Image* dest, const Image** gradients, const uint nbGradients) {
    // Il faudrait regarder pour l'image destination également ...
    // const int condition = Gx->width == Gy->width && Gx->height == Gy->height && Gx->channels == Gy->channels;
    // assert(condition && "Error::normGradient: given images do not have the same dimensions");

    const int chs = gradients[0]->channels;
    const int step = gradients[0]->width * chs * sizeof(uchar);

    for(uint i = 0; i < gradients[0]->height; ++i) {
        for(uint j = 0; j < gradients[0]->width; ++j) {
            const int idPixel = i * step + j * chs;

            for(int k = 0; k < chs; ++k) {
                // Calcul du carré du gradient pour le pixel
                float sumSquaredGradients = 0.f;
                for(uint g = 0; g < nbGradients; ++g) {
                    sumSquaredGradients += pow(gradients[g]->data[idPixel + k], 2);
                }
                
                const float result = sqrt(sumSquaredGradients);
                dest->data[idPixel + k] = (uchar) clampf(result);
            }
        }
    }
}