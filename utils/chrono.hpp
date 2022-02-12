#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.hpp"

typedef struct Chrono {
    clock_t begin;  // Temps avant début algorithme
    clock_t end;    // Temps après alogrithme
    double times;   // Tous les temps mesurés
    uint nbTimes;   // Nombre de temps mesurés
    clock_t clocksPerSecond;

    Chrono(const uint nbThreads, clock_t b = 0, clock_t e = 0, double ts = 0.0, uint nbTs = 0, clock_t cps = 0) {
        clocksPerSecond = CLOCKS_PER_SEC * nbThreads;
        begin = b;
        end = e;
        times = ts;
        nbTimes = nbTs;
    };

    void start();
    void stop();
    void printMeanTime();
    void printElapsedMSeconds();
    void printElapsedSeconds();
} Chrono;