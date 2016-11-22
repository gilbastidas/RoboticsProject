/******************************************
 * OpenCV Tutorial: Ball Tracking using   *
 * Kalman Filter                          *
 ******************************************/

// Module "core"
#include <opencv2/core/core.hpp>

// Module "highgui"
#include <opencv2/highgui/highgui.hpp>

// Module "imgproc"
#include <opencv2/imgproc/imgproc.hpp>

// Module "video"
#include <opencv2/video/video.hpp>

// Output
#include <iostream>

// Vector
#include <vector>

// Math
#include <math.h>

#include "aStar_original.h"

#define PI 3.14159265

using namespace std;

// >>>>> Color to be tracked
#define MIN_H_BLUE 200
#define MAX_H_BLUE 300
// <<<<< Color to be tracked

int distance(int x_1,int y_1,int x_2,int y_2){
	int res;
	res =sqrt(pow(x_1 - x_2, 2)+pow(y_1 - y_2, 2));
	return res;
}

int main()
{
    // Camera frame
    cv::Mat frame;

    // >>>> Kalman Filter
    int stateSize = 6;
    int measSize = 4;
    int contrSize = 0;

    unsigned int type = CV_32F;
    cv::KalmanFilter kf(stateSize, measSize, contrSize, type);

    cv::Mat state(stateSize, 1, type);  // [x,y,v_x,v_y,w,h]
    cv::Mat meas(measSize, 1, type);    // [z_x,z_y,z_w,z_h]
    //cv::Mat procNoise(stateSize, 1, type)
    // [E_x,E_y,E_v_x,E_v_y,E_w,E_h]

	// Transition State Matrix A
	// Note: set dT at each processing step!
	// [ 1 0 dT 0  0 0 ]
	// [ 0 1 0  dT 0 0 ]
	// [ 0 0 1  0  0 0 ]
	// [ 0 0 0  1  0 0 ]
	// [ 0 0 0  0  1 0 ]
	// [ 0 0 0  0  0 1 ]
    cv::setIdentity(kf.transitionMatrix);

    // Measure Matrix H
    // [ 1 0 0 0 0 0 ]
    // [ 0 1 0 0 0 0 ]
    // [ 0 0 0 0 1 0 ]
    // [ 0 0 0 0 0 1 ]
    kf.measurementMatrix = cv::Mat::zeros(measSize, stateSize, type);
    kf.measurementMatrix.at<float>(0) = 1.0f;
    kf.measurementMatrix.at<float>(7) = 1.0f;
    kf.measurementMatrix.at<float>(16) = 1.0f;
    kf.measurementMatrix.at<float>(23) = 1.0f;

    // Process Noise Covariance Matrix Q
    // [ Ex   0   0     0     0    0  ]
    // [ 0    Ey  0     0     0    0  ]
    // [ 0    0   Ev_x  0     0    0  ]
    // [ 0    0   0     Ev_y  0    0  ]
    // [ 0    0   0     0     Ew   0  ]
    // [ 0    0   0     0     0    Eh ]
    //cv::setIdentity(kf.processNoiseCov, cv::Scalar(1e-2));
    kf.processNoiseCov.at<float>(0) = 1e-2;
    kf.processNoiseCov.at<float>(7) = 1e-2;
    kf.processNoiseCov.at<float>(14) = 5.0f;
    kf.processNoiseCov.at<float>(21) = 5.0f;
    kf.processNoiseCov.at<float>(28) = 1e-2;
    kf.processNoiseCov.at<float>(35) = 1e-2;

    // Measures Noise Covariance Matrix R
    cv::setIdentity(kf.measurementNoiseCov, cv::Scalar(1e-1));
    // <<<< Kalman Filter

    // Camera Index
    int idx = 1;

    // Camera Capture
    cv::VideoCapture cap;

    // >>>>> Camera Settings
    if (!cap.open(idx))
    {
        cout << "Webcam not connected.\n" << "Please verify\n";
        return EXIT_FAILURE;
    }

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1024);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 768);
    // <<<<< Camera Settings

    cout << "\nHit 'q' to exit...\n";

    char ch = 0;

    double ticks = 0;
    bool found = false;

    int notFoundCount = 0;

    // >>>>> Main loop
    while (ch != 'q' && ch != 'Q')
    {
        double precTick = ticks;
        ticks = (double) cv::getTickCount();

        double dT = (ticks - precTick) / cv::getTickFrequency(); //seconds

        // Frame acquisition
        cap >> frame;

        cv::Mat res;
        frame.copyTo( res );

        if (found)
        {
            // >>>> Matrix A
            kf.transitionMatrix.at<float>(2) = dT;
            kf.transitionMatrix.at<float>(9) = dT;
            // <<<< Matrix A

            //cout << "dT:" << endl << dT << endl;

            state = kf.predict();
            //cout << "State post:" << endl << state << endl;

            cv::Rect predRect;
            predRect.width = state.at<float>(4);
            predRect.height = state.at<float>(5);
            predRect.x = state.at<float>(0) - predRect.width / 2;
            predRect.y = state.at<float>(1) - predRect.height / 2;

            cv::Point center;
            center.x = state.at<float>(0);
            center.y = state.at<float>(1);
            //cv::circle(res, center, 2, CV_RGB(255,0,0), -1);

           // cv::rectangle(res, predRect, CV_RGB(255,0,0), 2);
        }

        // >>>>> Noise smoothing
        cv::Mat blur;
        cv::GaussianBlur(frame, blur, cv::Size(5, 5), 3.0, 3.0);
        // <<<<< Noise smoothing

        // >>>>> HSV conversion
        cv::Mat frmHsv;
        cv::Mat frmHsv2;
        cv::cvtColor(blur, frmHsv, CV_BGR2HSV);
        cv::cvtColor(blur, frmHsv2, CV_BGR2HSV);
        // <<<<< HSV conversion

        // >>>>> Color Thresholding
        // Note: change parameters for different colors
        cv::Mat rangeRes = cv::Mat::zeros(frame.size(), CV_8UC1);
        cv::Mat rangeResOrange = cv::Mat::zeros(frame.size(), CV_8UC1);


        cv::inRange(frmHsv, cv::Scalar(MIN_H_BLUE / 2, 100, 80),
                    cv::Scalar(MAX_H_BLUE / 2, 255, 255), rangeRes);
//        cv::inRange(frmHsv2, cv::Scalar(126,89,0),
//        			cv::Scalar(179, 255, 255), rangeResOrange);
        //Orange detection parameters 13:55q
        //cv::inRange(frmHsv2, cv::Scalar(0,91,183), // Really good parameters at 15:00 of november, but it detects blue triangle also u.u
        //        			cv::Scalar(179, 255, 255), rangeResOrange);
        cv::inRange(frmHsv2, cv::Scalar(0,97,205), // Really really good parameters at 17:23 of november, and it doesn't detect blue triangle
                        			cv::Scalar(179, 255, 255), rangeResOrange);
        // <<<<< Color Thresholding

        // >>>>> Improving the result
        cv::erode(rangeRes, rangeRes, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(rangeRes, rangeRes, cv::Mat(), cv::Point(-1, -1), 2);

        cv::erode(rangeResOrange, rangeResOrange, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(rangeResOrange, rangeResOrange, cv::Mat(), cv::Point(-1, -1), 2);

        // <<<<< Improving the result

        // Thresholding viewing
        cv::imshow("Threshold", rangeRes);
        cv::imshow("Threshold_Orange", rangeResOrange);

        // >>>>> Contours detection
        vector<vector<cv::Point> > contours;
        vector<vector<cv::Point> > triangleContours;

        vector<cv::Point> result;
        cv::findContours(rangeResOrange, contours, CV_RETR_EXTERNAL,
                         CV_CHAIN_APPROX_NONE);
        cv::findContours(rangeRes, triangleContours, CV_RETR_LIST,
                                 CV_CHAIN_APPROX_SIMPLE);
        // <<<<< Contours detection

        cv::Point triCenter;
        cv::Point triEdge;

        for (size_t i = 0; i < triangleContours.size(); i++) {
        	cv::approxPolyDP(cv::Mat(triangleContours[i]), result, cv::arcLength(cv::Mat(triangleContours[i]), true)*0.02, true);
        	if(result.size() == 4){
        		float l1, l2, l3, l4;
        		l1 = distance(result[0].x, result[0].y, result[1].x, result[1].y);
        		l2 = distance(result[1].x, result[1].y, result[2].x, result[2].y);
        		l3 = distance(result[2].x, result[2].y, result[3].x, result[3].y);
        		l4 = distance(result[3].x, result[3].y, result[0].x, result[0].y);

        		//Edger result from the average of two points
        		cv::Point edgeResult;
        		vector<cv::Point>  newEdges;

        		// Finding closest points
        		if(l1 < l2 && l1 < l3 && l1 < l4){
        			//Construct new array
        			edgeResult.x = (result[0].x + result[1].x)/2;
        			edgeResult.y = (result[0].y + result[1].y)/2;
        			newEdges.push_back(edgeResult);
        			newEdges.push_back(result[2]);
        			newEdges.push_back(result[3]);

        		}
        		else if(l2 < l1 && l2 < l3 && l2 < l4){
        			edgeResult.x = (result[1].x + result[2].x)/2;
					edgeResult.y = (result[1].y + result[2].y)/2;
					newEdges.push_back(edgeResult);
					newEdges.push_back(result[0]);
					newEdges.push_back(result[3]);
        		}
        		else if(l3 < l1 && l3 < l2 && l3 < l4){
        			edgeResult.x = (result[2].x + result[3].x)/2;
        			edgeResult.y = (result[2].y + result[3].y)/2;
        			newEdges.push_back(edgeResult);
					newEdges.push_back(result[0]);
					newEdges.push_back(result[1]);
        		}
        		else {
        			edgeResult.x = (result[3].x + result[0].x)/2;
        			edgeResult.y = (result[3].y + result[0].y)/2;
        			newEdges.push_back(edgeResult);
					newEdges.push_back(result[1]);
					newEdges.push_back(result[2]);
        		}
        		result = newEdges;
        	}



        	if(result.size() == 3){

				cv::Rect roundApx;
				roundApx = cv::boundingRect(result);
				float ratio = (float) roundApx.width / (float) roundApx.height;
				if(ratio > 1.0f)
					ratio = 1.0f/ratio;

				// Ratio is the rectangularity of the contour, while closer to one is more squared, closer to 0 is more rectangular
				// Area is the space within the rectangle contour
        		if(ratio > 0.5 && roundApx.area()>= 150){
        			cout << "Triangle Found!" << endl;
					cv::line(res,result[0],result[1], CV_RGB(0,255,0),4);
					cv::line(res,result[1],result[2], CV_RGB(0,255,0),4);
					cv::line(res,result[0],result[2], CV_RGB(0,255,0),4);
					cv::circle(res, result[0], 10, CV_RGB(150,20,20), -1);
					cv::circle(res, result[1], 10, CV_RGB(150,20,20), -1);
					cv::circle(res, result[2], 10, CV_RGB(150,20,20), -1);
					triCenter.x = (result[0].x+result[1].x+result[2].x)/3;
					triCenter.y = (result[0].y+result[1].y+result[2].y)/3;
					cv::circle(res, triCenter, 10, CV_RGB(255,164,6), -1);

					if(distance(result[0].x,result[0].y,result[1].x,result[1].y)<distance(result[0].x,result[0].y,result[2].x,result[2].y) && distance(result[0].x,result[0].y,result[1].x,result[1].y)<distance(result[1].x,result[1].y,result[2].x,result[2].y)){
						triEdge = result[2];
					}else if(distance(result[2].x,result[2].y,result[1].x,result[1].y)<distance(result[0].x,result[0].y,result[2].x,result[2].y) && distance(result[2].x,result[2].y,result[1].x,result[1].y)<distance(result[0].x,result[0].y,result[1].x,result[1].y)){
						triEdge = result[0];
					}else{
						triEdge = result[1];
					}
					cv::line(res,triCenter,triEdge, CV_RGB(255,164,6),2);

					stringstream coordinatesCenter;
					coordinatesCenter << "P(" << triCenter.x << "," << triCenter.y << ")";
					cv::putText(res, coordinatesCenter.str(),
							triCenter, cv::FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(109,232,39), 1);

					stringstream coordinatesEdge;
					coordinatesEdge << "P(" << triEdge.x << "," << triEdge.y << ")";
							cv::putText(res, coordinatesEdge.str(),
									triEdge, cv::FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(109,232,39), 1);

					//atan2
					stringstream angle;
					//angle << atan2(10,2)*180 / PI;
					//angle << atan2(triEdge.y-triCenter.y,triEdge.x-triCenter.x);
					//angle << -1*(atan2(triEdge.y-triCenter.y,triEdge.x-triCenter.x)*180 / PI);
					int direction = -1*(atan2(triEdge.y-triCenter.y,triEdge.x-triCenter.x)*180 / PI);
					if ((direction <= 22.5) && (direction >= -22.5)){
							cv::putText(res, "East",
									triEdge, cv::FONT_HERSHEY_SIMPLEX, 0.7, CV_RGB(255,0,255), 2);
					}else if(direction < 67.5 && direction > 22.5){
						cv::putText(res, "North-East",
									triEdge, cv::FONT_HERSHEY_SIMPLEX, 0.7, CV_RGB(255,0,255), 2);
					}else if(direction <= 112.5 && direction >= 67.5){
						cv::putText(res, "North",
									triEdge, cv::FONT_HERSHEY_SIMPLEX, 0.7, CV_RGB(255,0,255), 2);
					}else if(direction < 157.5 && direction > 112.5){
						cv::putText(res, "North-West",
									triEdge, cv::FONT_HERSHEY_SIMPLEX, 0.7, CV_RGB(255,0,255), 2);
					}else if(direction >= 157.5 || direction <= -157.5){
						cv::putText(res, "West",
									triEdge, cv::FONT_HERSHEY_SIMPLEX, 0.7, CV_RGB(255,0,255), 2);
					}else if(direction < -22.5 && direction > -67.5){
						cv::putText(res, "South-East",
									triEdge, cv::FONT_HERSHEY_SIMPLEX, 0.7, CV_RGB(255,0,255), 2);
					}else if(direction <= -67.5 && direction >= -112.5){
						cv::putText(res, "South",
									triEdge, cv::FONT_HERSHEY_SIMPLEX, 0.7, CV_RGB(255,0,255), 2);
					}else if(direction < -112.5 && direction > -157.5){
						cv::putText(res, "South-West",
									triEdge, cv::FONT_HERSHEY_SIMPLEX, 0.7, CV_RGB(255,0,255), 2);
					}
        		}


			}else{
				cout << "Corners found:" << result.size() << endl;
//				int lowestdistance = 100000000;
//				for(int i=0;i<result.size();i++){
//					for(int j = i+1;j<result.size();j++){
//
//					}
//				}
			}

        }

        vector<vector<cv::Point> >  obstacles;
        vector<cv::Rect>  obstaclesBox;
        for (size_t i = 0; i < contours.size(); i++) {
        	cv::Rect oBox;
        	oBox = cv::boundingRect(contours[i]);
        	float ratio = (float) oBox.width / (float) oBox.height;
        	if(ratio > 1.0f)
        		ratio = 1.0f/ratio;

        	if(ratio > 0.5 && oBox.area()>= 300){
        		obstacles.push_back(contours[i]);
        		obstaclesBox.push_back(oBox);

        	}
        }
        vector<cv::Point>  centers;
        // >>>>> Detection result
        for (size_t i = 0; i < obstacles.size(); i++){
        	cv::drawContours(res, obstacles, i, CV_RGB(20,150,20), 1);
        	cv::rectangle(res, obstaclesBox[i], CV_RGB(0,255,0), 2);

        	cv::Point center;
            center.x = obstaclesBox[i].x + obstaclesBox[i].width / 2;
            center.y = obstaclesBox[i].y + obstaclesBox[i].height / 2;
            cv::circle(res, center, 2, CV_RGB(20,150,20), -1);
            centers.push_back(center);
//            stringstream sstr;
//            sstr << "(" << center.x << "," << center.y << ")";
//            cv::putText(res, sstr.str(),
//                     cv::Point(center.x + 3, center.y - 3),
//                     cv::FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(20,150,20), 2);
        }
                // <<<<< Detection result

        //Drawing grid
        int width=res.size().width;
        int height=res.size().height;
        int rows = 10;
        int columns = 20;
        int w = width/columns;
        int h = height/rows;
        for (int i=0; i<height; i+=h)
        	cv::line(res,cv::Point(0,i),cv::Point(width,i), CV_RGB(255,164,6),1, 8);

        for (int i=0; i<width; i+=w)
        	cv::line(res, cv::Point(i,0), cv::Point(i,height), CV_RGB(255,164,6),1,8);

        //Screen's dimensions
        stringstream dimension_txt;
        //sstr << "(" << center.x << "," << center.y << ")";
        dimension_txt << "Width size:" << width << " Height size:" << height;
        cv::putText(res, dimension_txt.str(),
                                cv::Point(5, 20),
                                cv::FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(243,248,253), 1);


        int main_matrix [rows][columns];
        int n,m;
        int px = triCenter.x;
        int py = triCenter.y;
        for (n=0; n<rows; n++){
        	for (m=0; m<columns; m++)
        	{
        		//if (main_matrix[n][m] != 2)
        			main_matrix[n][m] = 0;
        	}
        }


          for (int i=0;i <centers.size();i++){
			  for (n=0; n<rows; n++){
				  for (m=0; m<columns; m++)
				  {
					if(centers[i].x > (w * m) && centers[i].x <= (w*(m+1)) && centers[i].y > (h * n) && centers[i].y <= (h*(n+1)) )
					{
						main_matrix[n][m]=2;
						//cout << main_matrix[n][m];
						//cout << "valor w: " <<w*(m+1) <<" m= " << m << "valor h: " << h*(n+1) <<endl;
					}
				  }
				  //cout << endl;
				}
          }
          for (n=0; n<rows; n++){
			  for (m=0; m<columns; m++)
			  {
				  if(main_matrix[n][m]==2){
					  for (int i=max(n-1,0); i<=min(n+1,rows); i++){
						  for (int j=max(m-1,0); j<=min(m+1,columns); j++){
							  main_matrix[i][j]=1;
						  }
					  }
				  }
			  }
		  }
          int temp_matrix[20][10];
		  for(int y=0;y<rows;y++)
		  {
          			  for(int x=0;x<columns;x++) temp_matrix[x][y]=main_matrix[y][x];
          }
          for (n=0; n<rows; n++){
            for (m=0; m<columns; m++)
            {
            	if(px > w * m && px <= w*(m+1) && py > h * n && py <= h*(n+1))
            	{
            		main_matrix[n][m]=3;
            		//cout << main_matrix[n][m];
            		//cout << "valor w: " <<w*(m+1) <<" m= " << m << "valor h: " << h*(n+1) <<endl;
            	}
            	else
            	{
            		//cout << main_matrix[n][m] ;
            		//cout << "valor w: " <<w*(m+1) <<" m= " << m << "valor h: " << h*(n+1) <<endl;
            	}


            }
            //cout << endl;
          }
          for (n=0; n<rows; n++){
			  for (m=0; m<columns; m++)
			  {
				  if(main_matrix[n][m]==2 ||main_matrix[n][m]==1)
					  main_matrix[n][m]=1;
			  }
		  }
		  for (n=0; n<rows; n++){
			  for (m=0; m<columns; m++)
			  {
				  cout << main_matrix[n][m];
			  }
			  cout << endl;
		  }



		  int startx = floor(triCenter.x/w);
		  int starty = floor(triCenter.y/h);
		  int finishx =0;
		  int finishy =0;
		  string path = aStar(temp_matrix,startx,starty,finishx,finishy);
		  cout<<"Route:"<<endl;
		  cout<<path<<endl<<endl;
		  cv::Point startCoordinate = triCenter;
		  cv::Point nextCoordinate;

		  for (n=0; n<rows; n++){
			  for (m=0; m<columns; m++)
			  {
				if(px > w * m && px <= w*(m+1) && py > h * n && py <= h*(n+1) && path.length()>0)
				{
					//Declarar la nueva coordenada de inicio, segun la ubicacion del centroide, ubicarla en el centro del rectangulo
					startCoordinate = cv::Point((2*w*m+w)/2, (2*h*n+h)/2);
					cv::circle(res, startCoordinate, 5, CV_RGB(255,0,0), -1);

//					for(int i=0; i<=path.length(); i++){
//						cout<<"Path por separado: "<< path.at(i);
//						if(selectedChar == "L"){
//							nextCoordinate = cv::Point((2*w*m-w)/2, (2*h*n+h)/2);

					// Get the following coordinate
						for(int i=0;i<path.length();i++)
					        {
								char selectedChar = path.at(i);
									if(selectedChar == 'W'){
										//nextCoordinate = cv::Point((2*w*m-w)/2, (2*h*n+h)/2);
										nextCoordinate = cv::Point(startCoordinate.x - w, startCoordinate.y);
										cv::line(res,startCoordinate,nextCoordinate, CV_RGB(0,255,0),2);
										cv::circle(res, nextCoordinate, 5, CV_RGB(255,0,0), -1);
										startCoordinate = nextCoordinate;
										//cout<<"Encontre un W"<< path.at(i);
									}
									else if(selectedChar == 'N'){
										nextCoordinate = cv::Point(startCoordinate.x, startCoordinate.y-h);
										cv::line(res,startCoordinate,nextCoordinate, CV_RGB(0,255,0),2);
										cv::circle(res, nextCoordinate, 5, CV_RGB(255,0,0), -1);
										startCoordinate = nextCoordinate;
										//cv::circle(res, startCoordinate, 10, CV_RGB(255,0,0), -1);
										//cout<<"Encontre un N"<< path.at(i);
									}
									else if(selectedChar == 'S'){
										nextCoordinate = cv::Point(startCoordinate.x, startCoordinate.y+h);
										cv::line(res,startCoordinate,nextCoordinate, CV_RGB(0,255,0),2);
										cv::circle(res, nextCoordinate, 5, CV_RGB(255,0,0), -1);
										startCoordinate = nextCoordinate;
										//cv::circle(res, startCoordinate, 10, CV_RGB(255,0,0), -1);
										//cout<<"Encontre un S"<< path.at(i);
									}
									else {
										nextCoordinate = cv::Point(startCoordinate.x+w, startCoordinate.y);
										cv::line(res,startCoordinate,nextCoordinate, CV_RGB(0,255,0),2);
										cv::circle(res, nextCoordinate, 5, CV_RGB(255,0,0), -1);
										startCoordinate = nextCoordinate;
									}



					        }



					//cout << main_matrix[n][m];
					//cout << "valor w: " <<w*(m+1) <<" m= " << m << "valor h: " << h*(n+1) <<endl;
				}
				else
				{
					//cout << main_matrix[n][m] ;
					//cout << "valor w: " <<w*(m+1) <<" m= " << m << "valor h: " << h*(n+1) <<endl;
				}


			  }
			  //cout << endl;
			}


        // >>>>> Filtering
        vector<vector<cv::Point> > balls;
        vector<cv::Rect> ballsBox;
//        for (size_t i = 0; i < contours.size(); i++)
//        {
//            cv::Rect bBox;
//            bBox = cv::boundingRect(contours[i]);
//
//            float ratio = (float) bBox.width / (float) bBox.height;
//            if (ratio > 1.0f)
//                ratio = 1.0f / ratio;
//
//            // Searching for a bBox almost square
//            if (ratio > 0.75 && bBox.area() >= 400)
//            {
//                balls.push_back(contours[i]);
//                ballsBox.push_back(bBox);
//            }
//        }
        // <<<<< Filtering

        //cout << "Balls found:" << ballsBox.size() << endl;

//        if(result.size() == 3){
//        	cout << "Triangle Found!" << endl;
//			cv::Point tCenter;
//
//			for (size_t i = 0; i < result.size(); i++)
//			{
//				cv::drawContours(res, result, i, CV_RGB(20,150,20), 1);
//			}
//		}



        // >>>>> Kalman Update
        if (balls.size() == 0)
        {
            notFoundCount++;
            //cout << "notFoundCount:" << notFoundCount << endl;
            if( notFoundCount >= 100 )
            {
                found = false;
            }
            /*else
                kf.statePost = state;*/
        }
        else
        {
            notFoundCount = 0;

            meas.at<float>(0) = ballsBox[0].x + ballsBox[0].width / 2;
            meas.at<float>(1) = ballsBox[0].y + ballsBox[0].height / 2;
            meas.at<float>(2) = (float)ballsBox[0].width;
            meas.at<float>(3) = (float)ballsBox[0].height;

            if (!found) // First detection!
            {
                // >>>> Initialization
                kf.errorCovPre.at<float>(0) = 1; // px
                kf.errorCovPre.at<float>(7) = 1; // px
                kf.errorCovPre.at<float>(14) = 1;
                kf.errorCovPre.at<float>(21) = 1;
                kf.errorCovPre.at<float>(28) = 1; // px
                kf.errorCovPre.at<float>(35) = 1; // px

                state.at<float>(0) = meas.at<float>(0);
                state.at<float>(1) = meas.at<float>(1);
                state.at<float>(2) = 0;
                state.at<float>(3) = 0;
                state.at<float>(4) = meas.at<float>(2);
                state.at<float>(5) = meas.at<float>(3);
                // <<<< Initialization

                found = true;
            }
            else
                kf.correct(meas); // Kalman Correction

           // cout << "Measure matrix:" << endl << meas << endl;
        }
        // <<<<< Kalman Update

        // Final result
        cv::imshow("Tracking", res);

        // User key
        ch = cv::waitKey(1);
    }
    // <<<<< Main loop

    return EXIT_SUCCESS;
}
