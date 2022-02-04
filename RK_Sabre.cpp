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
// Compilation : g++ `pkg-config opencv --cflags` ../RK_Sabre.cpp ../utils.cpp -o RK_Projet `pkg-config opencv --libs`

/*
 * Code written by Vinz (GeckoGeek.fr) 14/03/2010
 * http://www.geckogeek.fr/lire-le-flux-dune-webcam-camera-video-avec-opencv.html
 * And modified by Rostom Kachouri; 16/02/2016
 * IGM_M2_Image
 */

#include "src/utils.hpp"
#include "src/chrono.hpp"
#include "src/sobel.hpp"
#include "src/median.hpp"

int main(int argc, char* argv[])
{
	// Récupérer le numéro de caméra entré par l'utilisateur en paramètre
	uint choosenCamera = 0;
	int opt;
	while((opt = getopt(argc, argv, "c:")) != -1) {
		switch (opt) {
			case 'c':
				choosenCamera = atoi(optarg);
				break;
			
			default:
				printf("Usage: program -c <id of camera (uint)>\n");
				exit(EXIT_FAILURE);
				break;
		}
	}
	printf("Camera id: %d\n", choosenCamera);

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

	// Boucle tant que l'utilisateur n'appuie pas sur la touche q (ou Q)
    while(ESC_keyboard != 'q' && ESC_keyboard != 'Q') {
 
		// On récupère une Image_IN
		Image_IN = cvQueryFrame(capture);
		// Initialisation des pointeurs de donnée
		inputImage.data  = (uchar *) Image_IN->imageData;
		outputImage.data = (uchar *) Image_OUT->imageData;

		// Conversion de l'image d'entrée en niveaux de gris
		getGrayScaleImage(&inputImage, &outputImage);

		// Apply Median Filter
		// medianFilter(&outputImage, chrono, 1);

		// Apply Sobel Filter
		chrono.start();
		sobel(&outputImage);
		chrono.stop();

		// On affiche l'Image_IN dans une fenêtre
		cvShowImage( "Image_IN_Window", Image_IN);
		// On affiche l'Image_OUT dans une deuxième fenêtre
		cvShowImage( "Image_OUT_Window", Image_OUT);

		// On attend 5ms
		ESC_keyboard = cvWaitKey(5);
    }

	chrono.printMeanTime();

	// Fermeture de l'acquisition Vidéo
	cvReleaseCapture(&capture);

	// Fermeture des fenêtres d'affichage
	cvDestroyWindow("Image_IN_Window");
	cvDestroyWindow("Image_OUT_Window");

	return 0;
}


/*
* MedianFilter : 96ms
*	
*   1px
*   - bubble sort :   0.001020 ms
*   - quickselect :   0.001011 ms
*   - quickselect2 :  0.001129 ms
*
*   - getPixelKernel: 0.000799 ms

*  3px
*   - bubble sort :   0.008518 ms
*   - quickselect :   0.003928 ms
*   - quickselect2 :  0.003052 ms
*
*   - getPixelKernel: 0.001195 ms



* 3px 				--> 904.276633 ms
* 3px omp (1 for)   --> 843.336721 ms
* 3px omp (2 for)   --> 902.591213 ms



*/
