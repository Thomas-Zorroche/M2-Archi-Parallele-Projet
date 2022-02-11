#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Chrono {
    char* name;         // Nom du chrono
    clock_t begin = 0;  // Temps avant début algorithme
    clock_t end = 0;    // Temps après alogrithme
    double times = 0.0; // Tous les temps mesurés
    uint nbTimes = 0;   // Nombre de temps mesurés

    void start();
    void stop();
    void printMeanTime();
    void printElapsedMSeconds();
    void printElapsedSeconds();
} Chrono;