#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.hpp"

typedef struct Chrono {
    double begin;  // Temps avant début algorithme
    double end;    // Temps après alogrithme
    double times;   // Tous les temps mesurés
    uint nbTimes;   // Nombre de temps mesurés

    void start();
    void stop();
    void printMeanTime();
    void printElapsedMSeconds();
    void printElapsedSeconds();
} Chrono;