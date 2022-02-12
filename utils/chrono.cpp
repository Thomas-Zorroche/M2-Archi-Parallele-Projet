#include "chrono.hpp"

void Chrono::start() {
    begin = omp_get_wtime();
}

void Chrono::stop() {
    end = omp_get_wtime();
    times += end - begin;
    nbTimes++;
}

// Affiche le temps écoulé en ms
void Chrono::printElapsedMSeconds() {
    double elapsedTime = end - begin / 1000.0;
    printf("Elapsed time: %f ms\n", elapsedTime);
}

// Affiche le temps écoulé en s
void Chrono::printElapsedSeconds() {
    double elapsedTime = end - begin;
    printf("Elapsed time: %f s\n", elapsedTime);
}

// Affiche le temps moyen écoulé lors de l'éxécution de l'algo
void Chrono::printMeanTime() {
    double meanTime = times / (double) nbTimes;
    printf("Mean time: %lF ms\n", meanTime);
}