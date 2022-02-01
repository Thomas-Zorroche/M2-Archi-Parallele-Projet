#include "sobel.hpp"

void sobel(Image* image) {
    Image* Gx = copyImage(image);
    Image* Gy = copyImage(image);

    // On applique les filtres
    if(Gx->channels == 1) {
        applyConv2dGray(Gx, &sobelX);
        applyConv2dGray(Gy, &sobelY);
    } else {
        applyConv2dColor(Gx, &sobelX);
        applyConv2dColor(Gy, &sobelY);
    }

    // Norme du gradient
    normGradient(image, Gx, Gy);

    freeImage(Gx);
    freeImage(Gy);
}

void normGradient(const Image* dest, const Image* Gx, const Image* Gy) {
    // Il faudrait regarder pour l'image distination également ...
    const int condition = Gx->width == Gy->width && Gx->height == Gy->height && Gx->channels == Gy->channels;
    assert(condition && "Error::normGradient: given images do not have the same dimensions");

    const int chs = Gx->channels;
    const int step = Gx->width * chs * sizeof(uchar);

    for(uint i = 0; i < Gx->height; ++i) {
        for(uint j = 0; j < Gx->width; ++j) {
            const int idPixel = i * step + j * chs;

            for(int k = 0; k < chs; ++k) {
                const float result = sqrt(pow(Gx->data[idPixel + k], 2) + pow(Gy->data[idPixel + k], 2));
                dest->data[idPixel + k] = (uchar) clampf(result);
            }
        }
    }
}