#pragma once

#include <opencv/cv.h>

typedef struct CaptureInfo {
    int height;     // Hauteur de l'image
    int width;      // Largeur de l'image
    int channels;   // Nombre de cannaux
    int step;       // Distance entre les deux premiers pixels de lignes successives
    int step_gray;
} CaptureInfo;

void getGrayScaleImage(const uchar* imageIN, uchar* imageOut, const CaptureInfo& info);