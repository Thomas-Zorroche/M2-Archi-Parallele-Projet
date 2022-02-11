#include "median.hpp"

void medianFilter(Image* dest)
{
    Image* copy = copyImage(dest);
    uchar kernel[(1 + (2 * 3 /* kernelsize */)) * (1 + (2 * 3 /* kernelsize */))];
    int idPixel, pixelX, pixelY = 0;

    for(uint i = 0; i < dest->height * dest->width; ++i)
    {
        pixelX = i % dest->width;
        pixelY = i / dest->width;
        idPixel = pixelY * dest->width + pixelX;
        
        kernel[0] = copy->getDataAtPixel(idPixel);
        kernel[1] = copy->getDataAtPixel(idPixel + 1);
        kernel[2] = copy->getDataAtPixel(idPixel - 1);

        kernel[3] = copy->getDataAtPixel(idPixel + copy->width);
        kernel[4] = copy->getDataAtPixel(idPixel + copy->width + 1);
        kernel[5] = copy->getDataAtPixel(idPixel + copy->width - 1);

        kernel[6] = copy->getDataAtPixel(idPixel - copy->width);
        kernel[7] = copy->getDataAtPixel(idPixel - copy->width + 1);
        kernel[8] = copy->getDataAtPixel(idPixel - copy->width - 1);
        
        dest->data[i] = opt_med9(kernel);
    }

    freeImage(copy);
}