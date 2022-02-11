#include "sobel.hpp"

void sobel(Image* image) {
    Image* G0 = copyImage(image);
    Image* G90 = copyImage(image);
    Image* G45 = copyImage(image);
    Image* G135 = copyImage(image);

    // On applique les filtres
    // applyConv2dGray(G0, &sobel0);
    // applyConv2dGray(G90, &sobel90);
    // applyConv2dGray(G45, &sobel45);
    // applyConv2dGray(G135, &sobel135);

    applyConv2dGray_OPTI_1(G0, &sobel0);
    applyConv2dGray_OPTI_1(G90, &sobel90);
    applyConv2dGray_OPTI_1(G45, &sobel45);
    applyConv2dGray_OPTI_1(G135, &sobel135);

    // Norme du gradient
    const Image* gradients[4] = {G0, G90, G45, G135};
    // normGradient(image, gradients, 4);
    normGradient_OPTI_1(image, gradients); // Toujours utiliser 4 gradients

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

// Toujours utiliser 4 gradients
void normGradient_OPTI_1(const Image* dest, const Image** gradients) {
    const int chs = dest->channels;
    const int step = dest->width * chs * sizeof(uchar);
    float sumAbsGradients;
    int idPixel;

    for(uint i = 0; i < dest->height; ++i) {
        for(uint j = 0; j < dest->width; ++j) {
            idPixel = i * step + j * chs;

            sumAbsGradients = 0.f;
            sumAbsGradients += std::abs(gradients[0]->data[idPixel]);
            sumAbsGradients += std::abs(gradients[1]->data[idPixel]);
            sumAbsGradients += std::abs(gradients[2]->data[idPixel]);
            sumAbsGradients += std::abs(gradients[3]->data[idPixel]);
            
            dest->data[idPixel] = (uchar) clampf(sumAbsGradients);
        }
    }
}