//#include "cv.h"
//#include "highgui.h"
//#include "opencv2/opencv.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include <iostream>
//using namespace std;
//
//
////IplImage es tomada de la libreria de procesamiento de imagenes de inter, en la cual es un formato nativo.
////OpenCV solo soporta un conjunto de posibles formatos IplImage: align, alphaChannel, dataOrder, depth, height (image in pixels),
////Link: http://docs.opencv.org/master/d6/d5b/structIplImage.html http://stackoverflow.com/questions/17307430/explanation-of-iplimage-img
////En este caso estamos definiendo una variable llamada imgTracking y asignando el valor 0. Es decir es una variable ocn una estructura que contiene
////datos de la imagen en si, asi como alguna informacion adicional tal como (dimension, profundidad de color y demas propiedades descritas anteriormente).
//
//IplImage* imgTracking=0;
//
//int lastX1 = -1;
//int lastY1 = -1;
//
//int lastX2 = -1;
//int lastY2 = -1;
//
//
//float distance(int x1, int y1, int x2, int y2 ){
//	float dist;
//	dist = sqrt(pow(2,(x2-x1)) + pow(2,(y2-y1)));
//	return dist;
//}
//
//
//void trackObject(IplImage* imgThresh){
//	//CvSeq* define un arreglo llamado contour, es una secuancia dinamica de crecimiento
//	//http://docs.opencv.org/2.4/modules/core/doc/dynamic_structures.html
//	CvSeq* contour;  //hold the pointer to a contour
//	CvSeq* result;   //hold sequence of points of a contour
//	//Define un almacenamiento para varias estructuras dinamicas en OpenCV tales como CvSeq, CvSet, etc.
//	CvMemStorage *storage = cvCreateMemStorage(0); //storage area for all contours, crea un area de almacenamiento de memoria.
//	//Si el bloque es 0, el tamano del bloque es el predeterminado, es decir, alrededor de 64k.
//
//	//finding all contours in the image
//
//	//C++: void findContours(InputOutputArray image, OutputArrayOfArrays contours, OutputArray hierarchy, int mode, int method, Point offset=Point())
//	//C++: void findContours(InputOutputArray image, OutputArrayOfArrays contours, int mode, int method, Point offset=Point())
//	//Parametros; imagen: es la fuente, un solo canal de 8 bits, se puede usar compare(), inRange(), threshold() entre otros, para crear una salida de imagen binaria
//	/* en escala de grises o a color. La funcion modifica la imagen mientras extrae los contornos. Si el modo es igual a CV_RETR_CCOMP o CV_RETR_FLOODFILL, la entrada
//	 * puede tambien ser una imagen de un entero de etiquetas de 32 bits(CV_32SC1).
//	 * contours: contornos detectados, cada contorno es almacenado como un vector de puntos.
//	 * hierarchy: vector de salida opcional, contiene informacion acerca de la topologia de imagen. Tiene tantos elementos como numero de contornos, para cada ith-contorno
//	 * contours[i], la jerarquia de elemntos hierarchy[i][0], son conjuntos 0-based indices en contornos del siguiente y asi respectivamente.
//	 * si para el contorno i no hay siguiente, previo, padre o contorno anidado, los elementos de la hierharchy[i] seran negativos.
//	 * mode: modo de recuperacion de contornos;
//	 *	 	CV_RETR_EXTERNAL: recupera solo los contornos extremos exteriores, establece hierarchy[i][2]=hierarchy[i][3]=-1
//	 *	 	CV_RETR_LIST: recupera todos los contornos sin establecer ninguna relacion jerarquica
//	 * method: metodo de aproximacion de contorno.
//	 * 		CV_CHAIN_APPROX_SIMPLE: comprime horizontal, veritcal, y diagonalmente segmentos y deja solo sus puntos finales. Por ejemplo, el contorno
//	 * 		de un retangulo esta codificado con 4 puntos.
//	 * offset: offset opcional por el cual cada punto del contorno es cambiado. Es util si los contornos son extradidos de la imagen ROI y entonces deben ser analizados
//	 * en el contexto completo de la imagen 	 */
//	/* Sintaxis
//	 * cvDrawContours(CvArr* img, CvSeq* contour, CvScalar externalColor, CvScalar holeColor, int maxLevel, int thickness=1, int lineType=8 )
//	 * Link: http://docs.opencv.org/2.4/modules/core/doc/drawing_functions.html#void%20line(Mat&%20img,%20Point%20pt1,%20Point%20pt2,%20const%20Scalar&%20color,%20int%20thickness,%20int%20lineType,%20int%20shift)
//	 */
//	cvFindContours(imgThresh, storage, &contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
//	//cvContourArea(contour);
//	//Prueba, no se si esto me funcione para dibujar el contorno del triangulo
//	//cvDrawContours(imgThresh, contour, cvScalar(45,165,24), cvScalar(45,165,24), 100,8);
//	//cvDrawContours(imgTracking, contour, cvScalar(45,165,24), cvScalar(45,165,24), 100,4);
//	//iterating through each contour
//
//	while(contour)
//	{
//		//obtain a sequence of points of the countour, pointed by the variable 'countour'
//		result = cvApproxPoly(contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contour)*0.02, 0);
//
//		//if there are 3 vertices  in the contour and the area of the triangle is more than 100 pixels
//		if(result->total==3 && fabs(cvContourArea(result, CV_WHOLE_SEQ))>100 )
//		{
//			//iterating through each point
//			CvPoint *pt[3];
//			for(int i=0;i<3;i++){
//				pt[i] = (CvPoint*)cvGetSeqElem(result, i);
//			}
//
//			int posX=( pt[0]->x + pt[1]->x + pt[2]->x )/3;
//			int posY=( pt[0]->y + pt[1]->y + pt[2]->y )/3;
//
//			int vertice;
//
//			if(distance(pt[0]->x,pt[0]->y,pt[1]->x,pt[1]->y) < distance(pt[1]->x,pt[1]->y,pt[2]->x,pt[2]->y) && distance(pt[0]->x,pt[0]->y,pt[1]->x,pt[1]->y) < distance(pt[0]->x,pt[0]->y,pt[2]->x,pt[2]->y))
//			{
//				vertice = 2;
//			}else if(distance(pt[2]->x,pt[2]->y,pt[1]->x,pt[1]->y) < distance(pt[1]->x,pt[1]->y,pt[0]->x,pt[0]->y) && distance(pt[2]->x,pt[2]->y,pt[1]->x,pt[1]->y) < distance(pt[0]->x,pt[0]->y,pt[2]->x,pt[2]->y))
//			{
//				vertice = 0;
//			}else{
//				vertice = 1;
//			}
//
//
//			//if(posX > 360 ){
//				if(lastX1>=0 && lastY1>=0 && posX>=0 && posY>=0)
//				{
//					// Draw a red line from the previous point to the current point
//					cvLine(imgTracking, cvPoint(posX, posY), cvPoint(lastX1, lastY1), cvScalar(0,0,255), 4);
//
//					//drawing lines around the triangle
////					cvLine(imgTracking, *pt[0],*pt[1], cvScalar(55,0,0),5);
////					cvLine(imgTracking, *pt[1],*pt[2],cvScalar(55,0,0),5);
////		 			cvLine(imgTracking, *pt[2], *pt[0], cvScalar(55,0,0),5);
//				}
//
//				lastX1 = posX;
//				lastY1 = posY;
//
//				cvLine(imgTracking, cvPoint(posX, posY), cvPoint(pt[vertice]->x, pt[vertice]->y), cvScalar(0,255,0), 4);
//
//
//			//}
////			else{
////				if(lastX2>=0 && lastY2>=0 && posX>=0 && posY>=0)
////				{
////					// Draw a blue line from the previous point to the current point
////					cvLine(imgTracking, cvPoint(posX, posY), cvPoint(lastX2, lastY2), cvScalar(255,0,0), 4);
////				}
////
////				lastX2 = posX;
////				lastY2 = posY;
////			}
//		}
//
//		//obtain the next contour
//		contour = contour->h_next;
//	}
//
//	cvReleaseMemStorage(&storage);
//}
//
//
//
//
//int main(){
//    //load the video file to the memory
//	//CvCapture *capture =     cvCaptureFromAVI("E:/Projects/Robot/IESL Robot/robot/a.avi");
//	CvCapture *capture =     cvCaptureFromCAM(0);
//
//
//    if(!capture){
//        printf("Capture failure\n");
//        return -1;
//    }
//
//    IplImage* frame=0;
//    frame = cvQueryFrame(capture);
//    if(!frame) return -1;
//
//    //create a blank image and assigned to 'imgTracking' which has the same size of original video
//    imgTracking=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);
//    cvZero(imgTracking); //covert the image, 'imgTracking' to black
//
//    cvNamedWindow("Video");
//    cvNamedWindow("Triangle");
//
//	//iterate through each frames of the video
//    while(true){
//    	//imgTracking=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);
//    	//cvZero(imgTracking); //covert the image, 'imgTracking' to black
//    	cvSet(frame,CV_RGB(0,0,0));
//        frame = cvQueryFrame(capture);
//        if(!frame) break;
//        frame=cvCloneImage(frame);
//
//		//smooth the original image using Gaussian kernel
//        cvSmooth(frame, frame, CV_GAUSSIAN,3,3);
//
//		//converting the original image into grayscale
//		IplImage* imgGrayScale = cvCreateImage(cvGetSize(frame), 8, 1);
//		cvCvtColor(frame,imgGrayScale,CV_BGR2GRAY);
//
//        //thresholding the grayscale image to get better results
//		cvThreshold(imgGrayScale,imgGrayScale,100,255,CV_THRESH_BINARY_INV);
//
//
//		//image dilatation
//		//cvDilate(const CvArr* src, CvArr* dst, IplConvKernel* element=NULL, int iterations=1 )
//		//Link: http://docs.opencv.org/2.4/modules/imgproc/doc/filtering.html?highlight=dilate#dilate
//		// Erosion y dilatacion de muestra, pero no he comprobado si funciona.
//		//cvDilate(imgGrayScale,imgGrayScale,NULL,40);
//		//cvErode(imgGrayScale,imgGrayScale,NULL,40);
//
//        //track the possition of the ball
//        trackObject(imgGrayScale);
//
//        // Add the tracking image and the frame
//        cvAdd(frame, imgTracking, frame);
//
//        cvShowImage("Video", frame);
//		cvShowImage("Triangle", imgGrayScale);
//
//        //Clean up used images
//        cvReleaseImage(&imgGrayScale);
//        cvReleaseImage(&frame);
//
//        //Wait 10mS
//        int c = cvWaitKey(10);
//        //If 'ESC' is pressed, break the loop
//        if((char)c==27 ) break;
//
//
//    }
//
//
//
//    cvDestroyAllWindows();
//    cvReleaseImage(&imgTracking);
//    cvReleaseCapture(&capture);
//
//    return 0;
//}
//
