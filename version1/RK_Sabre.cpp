//
// RK_Sabre.cpp
//
// pour trouver le /dev/video? de la WebCam faire :
// sudo rm /dev/video*
// brancher la WebCam
// >> debian@SabreLite:~/Desktop$ ls /dev/video*
// ==>/dev/video4
//
// adapter la ligne :
//     capture = cvCreateCameraCapture( 4 );
//
// Compilation : g++ `pkg-config opencv --cflags` convolution.cpp median.cpp sobel.cpp ../utils/chrono.cpp ../utils/sort.cpp ../utils/utils.cpp RK_Sabre.cpp -o RK_Projet `pkg-config opencv --libs` -std=c++14 -fopenmp -pg

/*
 * Code written by Vinz (GeckoGeek.fr) 14/03/2010
 * http://www.geckogeek.fr/lire-le-flux-dune-webcam-camera-video-avec-opencv.html
 * And modified by Rostom Kachouri; 16/02/2016
 * IGM_M2_Image
 */

#include "../utils/utils.hpp"
#include "../utils/chrono.hpp"
#include "sobel.hpp"
#include "median.hpp"

int main(int argc, char* argv[])
{
	// Récupérer le numéro de caméra entré par l'utilisateur en paramètre
	uint choosenCamera 	= 0;
	uint numberOfFrames = 100;
	int opt;
	while((opt = getopt(argc, argv, "c:f:")) != -1) {
		switch (opt) {
			case 'c':
				choosenCamera = atoi(optarg);
				break;

			case 'f':
				numberOfFrames = atoi(optarg);
				break;
			
			default:
				printf("Usage: program -c <id of camera (uint)> -f <number of frames, 0 = infinity (uint)>\n");
				exit(EXIT_FAILURE);
				break;
		}
	}
	printf("Camera id: %d\n", choosenCamera);
	printf("Number of frames: %d\n", numberOfFrames);
	// Compteur de frames traitées
	uint framesCounter = 0;

	// Touche clavier pour quitter
	char ESC_keyboard;
	// Intialisation de la variable ESC_keyboard
	ESC_keyboard = cvWaitKey(5);

	// Images
	IplImage *Image_IN;
	IplImage *Image_OUT;

	// Outils pour la mesure des temps
	Chrono chrono;
		
	// Capture vidéo
	CvCapture *capture;

	// Ouvrir le flux vidéo
	// capture = cvCreateFileCapture("/path/to/your/video/test.avi"); // chemin pour un fichier
	// capture = cvCreateCameraCapture(CV_CAP_ANY);
	capture = cvCreateCameraCapture(choosenCamera);

	// Vérifier si l'ouverture du flux est ok
	if (!capture) {
		printf("Ouverture du flux vidéo impossible !\n");
		return 1;
	}

	// Définition des fenêtres
	cvNamedWindow("Image_IN_Window", CV_WINDOW_AUTOSIZE);   // Image_IN
	cvNamedWindow("Image_OUT_Window", CV_WINDOW_AUTOSIZE); 	// Image_OUT

	// Positionnement des fenêtres
	cvMoveWindow("Image_IN_Window", 0, 0);
	cvMoveWindow("Image_OUT_Window", 0, 700);

	// Première acquisition
	Image_IN = cvQueryFrame(capture);

	// Création de l'image de sortie
	Image_OUT = cvCreateImage(cvSize(Image_IN->width, Image_IN->height),  IPL_DEPTH_8U, 1);

	Image inputImage 	= {nullptr, (uint)Image_IN->height, (uint)Image_IN->width, Image_IN->nChannels};
	Image outputImage 	= {nullptr, (uint)Image_OUT->height, (uint)Image_OUT->width, Image_OUT->nChannels};

	// On définit le nombre de threads à utiliser pour les parallélisations
	omp_set_num_threads(NUM_THREADS);
	printf("Threads used: %d\n", NUM_THREADS);

	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
	chrono.start();
	printf("START Chrono\n");
    while(ESC_keyboard != 'q' && ESC_keyboard != 'Q' && framesCounter <= numberOfFrames) {
 
		// On récupère une Image_IN
		Image_IN = cvQueryFrame(capture);
		// Initialisation des pointeurs de donnée
		inputImage.data  = (uchar *) Image_IN->imageData;
		outputImage.data = (uchar *) Image_OUT->imageData;

		// Conversion de l'image d'entrée en niveaux de gris
		getGrayScaleImage(&inputImage, &outputImage);

		// Apply Median Filter
		medianFilter(&outputImage);

		// Apply Sobel Filter
		sobel(&outputImage, THRESHOLD);

		// On affiche l'Image_IN dans une fenêtre
		cvShowImage("Image_IN_Window", Image_IN);
		// On affiche l'Image_OUT dans une deuxième fenêtre
		cvShowImage("Image_OUT_Window", Image_OUT);

		// On attend 5ms
		ESC_keyboard = cvWaitKey(5);

		// Mise à jour du compteur de frames (sauf si 0 est sélectionné)
		if(!(numberOfFrames == 0)) {
			// printf("%d\n", framesCounter);
			framesCounter++;
		}
    }
	chrono.stop();
	printf("STOP Chrono\n");
	chrono.printElapsedSeconds();

	// Fermeture de l'acquisition Vidéo
	cvReleaseCapture(&capture);

	// Fermeture des fenêtres d'affichage
	cvDestroyWindow("Image_IN_Window");
	cvDestroyWindow("Image_OUT_Window");

	return 0;
}