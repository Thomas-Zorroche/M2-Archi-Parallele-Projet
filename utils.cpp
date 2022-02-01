#include "utils.hpp"

// TODO REMOVE THIS
#include <iostream>

 // Conversion RGB en niveau de gris
void getGrayScaleImage(const Image* imageIn, Image* imageOut) {
    const int chs = imageIn->channels;
    const int stepImageIn = imageIn->width * chs * sizeof(uchar);
    const int stepImageOut = imageOut->width * imageOut->channels * sizeof(uchar);

    for(uint i = 0; i < imageIn->height; ++i) {
        for(uint j = 0; j < imageIn->width; ++j) {
            const int idPixel = i * stepImageIn + j * chs;
            imageOut->data[i * stepImageOut + j] = 0.114 * imageIn->data[idPixel + 0] + 0.587 * imageIn->data[idPixel + 1] + 0.299 * imageIn->data[idPixel + 2];
        }
    }
}

void sobel(Image* image) {
    Image* Gx = copyImage(image);
    Image* Gy = copyImage(image);

    // On applique les filtres
    if(Gx->channels == 1) {
        applyFilterGray(Gx, &sobelX);
        applyFilterGray(Gy, &sobelY);
    } else {
        applyFilterColor(Gx, &sobelX);
        applyFilterColor(Gy, &sobelY);
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

    for(int i = 0; i < Gx->height; ++i) {
        for(int j = 0; j < Gx->width; ++j) {
            const int idPixel = i * step + j * chs;

            for(int k = 0; k < chs; ++k) {
                const float result = sqrt(pow(Gx->data[idPixel + k], 2) + pow(Gy->data[idPixel + k], 2));
                dest->data[idPixel + k] = (uchar) clampf(result);
            }
        }
    }
}

void applyFilterGray(const Image* dest, const Kernel* kernel) {
    // Copie temporaire des données de l'image
    Image* copy = copyImage(dest);
    const int step = dest->width * dest->channels * sizeof(uchar);

    for (int i = 0; i < dest->height; ++i) {
        for (int j = 0; j < dest->width; ++j) {
            const float convResult = conv2dGray(copy, {(float)j, (float)i}, kernel);
            dest->data[i * step + j] = (uchar) clampf(convResult);
        }
    }

    // On supprime la copie temporaire de l'image
    freeImage(copy);
}

void applyFilterColor(const Image* dest, const Kernel* kernel) {
    // Copie temporaire des données de l'image
    Image* copy = copyImage(dest);
    const int chs = dest->channels;
    const int step = dest->width * chs * sizeof(uchar);

    for (int i = 0; i < dest->height; ++i) {
        for (int j = 0; j < dest->width; ++j) {
            const float3 convResult = conv2dColor(copy, {(float)j, (float)i}, kernel);
            const int idPixel = i * step + j * chs;
            dest->data[idPixel + 0] = (uchar) clampf(convResult.x);
            dest->data[idPixel + 1] = (uchar) clampf(convResult.y);
            dest->data[idPixel + 2] = (uchar) clampf(convResult.z);
        }
    }

    // On supprime la copie temporaire de l'image
    freeImage(copy);
}

float conv2dGray(const Image* image, const Coord pixel, const Kernel* kernel) {
    const uint kernelWidth = kernel->width;
    const uint kernelHeight = kernel->height;
    const uint width = image->width;
    const uint height = image->height;
    const int chs = image->channels;
    const int step = image->width * chs * sizeof(uchar);

    float sum = 0.0f;

    for (uint i = 0; i < kernelHeight; ++i) {
        for (uint j = 0; j < kernelWidth; ++j) {
            int dX = pixel.x + i - kernelWidth / 2;
            int dY = pixel.y + j - kernelHeight / 2;

            // Handle borders
            if (dX < 0)
                dX = 0;

            if (dX >= width)
                dX = width - 1;

            if (dY < 0) 
                dY = 0;

            if (dY >= height)
                dY = height - 1;

            const int idKer		= j * kernelWidth + i;
            const int idPixel	= dY * step + dX;
            sum += image->data[idPixel] * kernel->data[idKer];
        }
    }

    return sum;
}

float3 conv2dColor(const Image* image, const Coord pixel, const Kernel* kernel) {
    const uint kernelWidth = kernel->width;
    const uint kernelHeight = kernel->height;
    const uint width = image->width;
    const uint height = image->height;
    const int chs = image->channels;
    const int step = image->width * chs * sizeof(uchar);

    float3 sum = {0.0f, 0.0f, 0.0f};

    for (uint i = 0; i < kernelHeight; ++i) {
        for (uint j = 0; j < kernelWidth; ++j) {
            int dX = pixel.x + i - kernelWidth / 2;
            int dY = pixel.y + j - kernelHeight / 2;

            // Handle borders
            if (dX < 0)
                dX = 0;

            if (dX >= width)
                dX = width - 1;

            if (dY < 0) 
                dY = 0;

            if (dY >= height)
                dY = height - 1;

            const int idKer		= j * kernelWidth + i;
            const int idPixel	= dY * step + dX * chs;
            sum.x += image->data[idPixel + 0] * kernel->data[idKer];
            sum.y += image->data[idPixel + 1] * kernel->data[idKer];
            sum.z += image->data[idPixel + 2] * kernel->data[idKer];
        }
    }

    return sum;
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

float clampf(const float value, const float max, const float min) {
    return (value <= min ? min : (value >= max ? max : value));
}

///////////////////////////////////////////////////////////////////////
// Median Filter
///////////////////////////////////////////////////////////////////////

uchar Image::getDataAtPixel(int idPixel)
{
    static const int numPixels = width * height * channels;

    if (idPixel < 0 || idPixel >= numPixels)
        return 0;

    return data[idPixel];
}

void swap(uchar *xp, uchar *yp)
{
    uchar temp = *xp;
    *xp = *yp;
    *yp = temp;
}
  
void bubbleSort(uchar arr[], int n)
{
   int i, j;
   for (i = 0; i < n-1; i++)      
  {
       // Last i elements are already in place   
       for (j = 0; j < n-i-1; j++) 
       {
           if (arr[j] > arr[j+1])
              swap(&arr[j], &arr[j+1]);
       }
  }
}

uchar getMedianValue(uchar kernel[], int n)
{
    // Sort array (bubble sort)
    bubbleSort(kernel, n);

    // Return median value
    return kernel[(int)n / 2];
}

void medianFilter(Image* image, int kernelSize)
{
    const int chs = image->channels; // 1
    const int step = image->width * chs /* * sizeof(uchar)*/;

    uchar kernel[(1 + (2 * kernelSize)) * (1 + (2 * kernelSize))];
    
    for(int i = 0; i < image->height; ++i) 
    {
        for(int j = 0; j < image->width; ++j) 
        {
            const int idPixel = i * step + j * chs;

            int idKernel = 0;
            for (int kx = -kernelSize; kx <= kernelSize; kx++)
            {
                for (int ky = -kernelSize; ky <= kernelSize; ky++)
                {
                    int idNeighbour = idPixel + (ky * chs) + (kx * image->width);
                    kernel[idKernel] = image->getDataAtPixel(idNeighbour);
                    idKernel++;
                } 
            }

            image->data[idPixel] = getMedianValue(kernel, idKernel);
        }
    }
    
}

void Chrono::start() { begin = clock(); }
void Chrono::stop() {
    end = clock();
    times += (double)(end - begin) / (double)CLOCKS_PER_SEC * 1000.0;
    nbTimes++;
}

// Affiche le temps écoulé en ms
void Chrono::printElapsedTime() {
    double elapsedTime = (double)(end - begin) / (double)CLOCKS_PER_SEC * 1000.0;
    printf("Elapsed time: %f ms\n", elapsedTime);
}

// Affiche le temps moyen écoulé lors de l'éxécution de l'algo
void Chrono::printMeanTime() {
    double meanTime = times / (double) nbTimes;
    printf("Mean time: %lF ms\n", meanTime);
}