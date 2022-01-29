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
		for (uint y = 0; y < imageIn->height; ++y)
		{
			for (uint x = 0; x < imageIn->width; ++x)
			{
                const float convResult = conv2d(imageIn, {(int)x, (int)y}, kernel);
				const int idOut = y * imageOut->width + x;
				imageOut->data[idOut] = (convResult <= 0.0f ? 0 : (convResult >= 255.0f ? 255 : (uchar)convResult));
			}
		}
}

float conv2d(const Image* image, const Coord& pixel, const Kernel* kernel) {
    const uint size = kernel->size;
    const uint width = image->width;
    const uint height = image->height;

    float sum = 0.0f;

    for (uint j = 0; j < size; ++j) {
        for (uint i = 0; i < size; ++i) {
            int dX = pixel.x + i - size / 2;
            int dY = pixel.y + j - size / 2;

            // Handle borders
            if (dX < 0)
                dX = 0;

            if (dX >= width)
                dX = width - 1;

            if (dY < 0) 
                dY = 0;

            if (dY >= height)
                dY = height - 1;

            const int idKer		= j * size + i;
            const int idPixel	= dY * width + dX;
            sum += image->data[idPixel] * kernel->data[idKer];
        }
    }

    return sum;
}