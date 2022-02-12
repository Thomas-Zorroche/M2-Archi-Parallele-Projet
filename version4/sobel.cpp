#include "sobel.hpp"

void sobel(Image* image, const float threshold) {
    float* Gx = (float*) malloc(sizeof(float) * image->width * image->height);
    float* Gy = (float*) malloc(sizeof(float) * image->width * image->height);

    // On applique les filtres
    sobelX(image, Gx, &sobel0);
    sobelY(image, Gy, &sobel90);

    // Norme du gradient
    const float* gradients[2] = {Gx, Gy};
    normGradient(image, gradients, threshold);

    free(Gx);
    free(Gy);
}

void normGradient(const Image* dest, const float** gradients, const float threshold) {
    float sumAbsGradients;
    uint id, idPixel;

    uint size = dest->height * dest->width;
    uint size_per_thread = size / NUM_THREADS;

    // Les valeurs déclarées const sont pré-déterminées 'shared'
    #pragma omp parallel shared(size, size_per_thread, dest, gradients) private(id, idPixel, sumAbsGradients)
    {
        id = omp_get_thread_num(); // Get current thread number
        idPixel = (size_per_thread * id);
        sumAbsGradients = 0.0f;

        #pragma omp for
        for(uint i = 0; i < size; ++i) {
            sumAbsGradients  = std::abs(gradients[0][idPixel]);
            sumAbsGradients += std::abs(gradients[1][idPixel]);
            
            dest->data[idPixel] = (uchar) clampfs(sumAbsGradients, threshold);
            idPixel++;
        }
    }
}