//#include <iostream>
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include <string>
//using namespace cv;
//using namespace std;
//
////FUNCIONA PERO NO ES LO QUE BUSCAMOS
//int theObject[2] = {0, 0};
////bounding rectangle of the object, we will use the center of this as its position
//Rect objectBoundingRectangle = Rect(0, 0, 0, 0);
//
//void searchForMovement(Mat thresholdImage, Mat &cameraFeed){
//	//notice how we use the & operator for the camera feed, this is because we wish
//	//to take the values passed into the function and manipulate them, rather than just working with a copy.
//	//eg. we draw to the cameraFeed in this functin which is then displayed in the main() function.
//
//	bool objectDetected = false;
//	Mat temp;
//	thresholdImage.copyTo(temp);
//	//these two vectors needed for output of findContours
//	vector< vector<Point> > contours;
//	vector<Vec4i> hierarchy;
//	//find contours of filtered image using openCV findContours function
//	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);//retrieve all contours
//
//	//if contours vector is not empty, we have found some objects
//	if (contours.size()>0)objectDetected = true;
//	else objectDetected =false;
//	if (objectDetected){
//		//the largest contour is found at the end of the contours vector
//		//we will simply assume that the biggest countour is the object we are looking for.
//		vector< vector<Point> > largestContourVec;
//		largestContourVec.push_back(contours.at(contours.size()-1));
//		//making a bounding rectangle around the largest contour then find its centroid
//		//this will be the objects final estimated position.
//		objectBoundingRectangle = boundingRect(largestContourVec.at(0));
//		int xpos = objectBoundingRectangle.x + objectBoundingRectangle.width/2;
//		int ypos = objectBoundingRectangle.y + objectBoundingRectangle.height/2;
//
//		//update the objects position by changint theObject array values
//		theObject[0] = xpos, theObject[1] = ypos;
//	}
//	//make some temp x and y variables so we dont have to type os to much
//	int x = theObject[0];
//	int y = theObject[1];
//	//draw some crosshairs on the object
//	circle(cameraFeed, Point(x,y), 20, Scalar(0, 255, 0), 2);
//	line(cameraFeed, Point(x,y), Point(x, y-25), Scalar(0, 255, 0), 2);
//	line(cameraFeed, Point(x,y), Point(x, y+25), Scalar(0, 255, 0), 2);
//	line(cameraFeed, Point(x,y), Point(x-25, y), Scalar(0, 255, 0), 2);
//	line(cameraFeed, Point(x,y), Point(x+25, y), Scalar(0, 255, 0), 2);
//
//	putText(cameraFeed, "Tracking object at ("+ x, Point(x,y), 1, 1, Scalar(0,0,255),2);
//}
//
//
//int main(int argc, char** argv)
//{
//	VideoCapture cap(1); //capture the video from web cam
//	if(!cap.isOpened()) //if not success, exit program
//	{
//		cout << "Cannot open the webcam" << endl;
//		return -1;
//	}
//
//	namedWindow("Control", CV_WINDOW_NORMAL); //create a windows called control
//
//	int iLowH = 0;
//	int iHighH = 179;
//
//	int iLowS = 0;
//	int iHighS = 255;
//
//	int iLowV = 0;
//	int iHighV = 255;
//
////create trackbars in control window
//	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0-179)
//	cvCreateTrackbar("HighH", "Control", &iHighH, 179);
//
//	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0-255)
//	cvCreateTrackbar("HighS", "Control", &iHighS, 255);
//
//	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Hue (0-179)
//	cvCreateTrackbar("HighV", "Control", &iHighV, 255);
//
//	while (true)
//	{
//		Mat imgOriginal;
//		bool bSuccess = cap.read(imgOriginal);//read a new frame from video
//
//		if(!bSuccess)//if not success, break loop
//		{
//			cout << "Cannot read a frame from video stream" << endl;
//			break;
//		}
//
//		Mat imgHSV;
//
//		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV);//conver the captured frame from bgr to hsv
//
//		Mat imgThresholded;
//
//		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //thresold the image
//		//morphological opening (remove small objects from the foreground)
//
//		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//		//morphological closing (fill small holes in the foreground)
//		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		searchForMovement(imgThresholded, imgOriginal);
//		imshow("Thresholded Image", imgThresholded);
//		imshow("Original", imgOriginal);
//
//		if (waitKey(30) == 27) //wait for esc key press for 30ms. If esc key is pressed, break loop
//		{
//			cout << "esc key is pressed by the user" << endl;
//			break;
//		}
//
//
//	}
//	return 0;
//}
