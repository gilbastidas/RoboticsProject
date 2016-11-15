//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/opencv.hpp"
//#include <iostream>
//
//
//using namespace cv;
//using namespace std;
//
///// Global Variables
//const int n_slider_max = 9; ///highest density grid will be max+1/max+1 = 10/10
//int n_slider;
//
///// Function header
//void MyLine( Mat img, Point start, Point end );
//void on_trackbar(int, void*);
//
//int main( int argc, char** argv ){
//
//    /// Initialize values
//    n_slider = 0;
//
//
//    //Capture the movie given as a command line argument
//    VideoCapture cap(argv[1]);
//
//
//    //create a window called "Video"
//    namedWindow("Video",CV_WINDOW_AUTOSIZE);
//
//    /// 2. Create Trackbar
//    char TrackbarName[100];
//    sprintf( TrackbarName, "N range [1,10]");
//
//    createTrackbar( TrackbarName, "Video", &n_slider, n_slider_max, on_trackbar );
//
//
//    // Movie Loop: loop to read video frames and display them in a window
//    while(0)
//    {
//        Mat frame;
//
//        bool bSuccess = cap.read(frame); // read a new frame from video
//
//        if (!bSuccess) //if not success, break loop
//        {
//            cout << "Cannot read the frame from video file" << endl;
//            break;
//        }
//
//
//        //Draw loop
//        for(int i=1; i<n_slider+1; ++i)
//        {//vertical lines
//            MyLine( frame, Point( frame.cols*i/(n_slider+1), 0 ), Point(frame.cols*i/(n_slider+1), frame.rows ) );
//
//            //horizontal lines
//            MyLine( frame, Point( 0, frame.rows*i/(n_slider+1) ), Point( frame.cols,frame.rows*i/(n_slider+1) ) );
//        }
//
//
//        //Show the movie
//        imshow("Video", frame);
//
//
//        if(waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
//        {
//            cout << "esc key is pressed by user" << endl;
//            break;
//        }
//
//    }
//
//    waitKey( 0 );
//    return(0);
//
//}
//
///// Function Declaration
//
///**
// * @function MyLine
// * @brief Draw a simple line
// */
//void MyLine( Mat img, Point start, Point end )
//{
//    int thickness = 1;
//    int lineType = 8;
//    line( img,
//         start,
//         end,
//         Scalar( 0, 0, 255 ),
//         thickness,
//         lineType );
//}
//
///**
// * @function on_trackbar
// * @brief Callback for trackbar
// */
//void on_trackbar( int, void* )
//{
//
//}
