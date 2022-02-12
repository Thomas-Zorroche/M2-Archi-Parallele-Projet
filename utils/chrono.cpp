#include "chrono.hpp"

void Chrono::start() {
    begin = clock();
}

void Chrono::stop() {
    end = clock();
    times += (double)(end - begin) / (double)clocksPerSecond * 1000.0;
    nbTimes++;
}

// Affiche le temps écoulé en ms
void Chrono::printElapsedMSeconds() {
    double elapsedTime = (double)(end - begin) / (double)clocksPerSecond * 1000.0;
    printf("Elapsed time: %f ms\n", elapsedTime);
}

// Affiche le temps écoulé en s
void Chrono::printElapsedSeconds() {
    double elapsedTime = (double)(end - begin) / (double)clocksPerSecond;
    printf("Elapsed time: %f s\n", elapsedTime);
}

// Affiche le temps moyen écoulé lors de l'éxécution de l'algo
void Chrono::printMeanTime() {
    double meanTime = times / (double) nbTimes;
    printf("Mean time: %lF ms\n", meanTime);
}