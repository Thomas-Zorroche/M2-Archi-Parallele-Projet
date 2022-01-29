#include "utils.hpp"

 // Conversion RGB en niveau de gris
void getGrayScaleImage(const Image* imageIn, Image* imageOut) {
    const int step = imageIn->step;
    const int chs = imageIn->channels;

    for(uint i = 0; i < imageIn->height; ++i) {
        for(uint j = 0; j < imageIn->width; ++j) {
            imageOut->data[i * imageOut->step_gray + j] = 0.114 * imageIn->data[i * step + j * chs + 0] + 0.587 * imageIn->data[i * step + j * chs + 1] + 0.299 * imageIn->data[i * step + j * chs + 2];
        }
    }
}

void applyFilter(const Image* imageIn, Image* imageOut, const Kernel* kernel) {
    const int width = imageIn->width;
    const int height = imageIn->height;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const int idOut = y * width + x;
            const float convResult = conv2d(imageIn, {x, y}, kernel);
            imageOut->data[idOut] = (convResult <= 0.0f ? 0 : (convResult >= 255.0f ? 255 : (uchar)convResult));
        }
    }
}

float conv2d(const Image* image, const Coord& pixel, const Kernel* kernel) {
    const uint kernelWidth = kernel->width;
    const uint kernelHeight = kernel->height;
    const uint width = image->width;
    const uint height = image->height;

    float sum = 0.0f;

    for (uint j = 0; j < kernelWidth; ++j) {
        for (uint i = 0; i < kernelHeight; ++i) {
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
            const int idPixel	= dY * width + dX;
            sum += image->data[idPixel] * kernel->data[idKer];
        }
    }

    return sum;
}