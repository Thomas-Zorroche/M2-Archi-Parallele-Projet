#include "utils.hpp"

 // Conversion RGB en niveau de gris
void getGrayScaleImage(const uchar* imageIN, uchar* imageOut, const CaptureInfo& info) {
    for(uint i = 0; i < info.height; ++i) {
        for(uint j = 0; j < info.width; ++j) {
            imageOut[i * info.step_gray + j]= 0.114 * imageIN[i * info.step + j * info.channels + 0] + 0.587 * imageIN[i * info.step + j * info.channels + 1] + 0.299 * imageIN[i * info.step + j * info.channels + 2];
        }
    }
}