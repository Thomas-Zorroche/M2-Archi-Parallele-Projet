# M2-Archi-Parallele-Projet
## Build le projet
* Créer un dossier `build/`, entrer dans ce dossier
* Créer un dossier par version : `mkdir version1 && mkdir version2 && mkdir version3 && mkdir version4`
* Aller dans un des dossiers et enter : `cmake ../../versionX`, avec X = la version
* Puis : `make -j`

Si CMake n'est pas disponible, voici la ligne de compilation à entrer :
``g++ `pkg-config opencv --cflags` convolution.cpp median.cpp sobel.cpp ../utils/chrono.cpp ../utils/sort.cpp ../utils/utils.cpp RK_Sabre.cpp -o RK_Projet `pkg-config opencv --libs` -std=c++14 -fopenmp -pg``
## Run le projet
* Enter : `./RK_Projet -c <id of camera (uint)> -f <number of frames, 0 = infinity (uint)>`