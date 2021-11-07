#define STB_IMAGE_IMPLEMENTATION    
#include "stb_image.h"
#include "opencv2/opencv.hpp"
#include "iostream"

using namespace cv;

int main(int, char**) {
    // open the first webcam plugged in the computer
    cv::VideoCapture camera(1);
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    // create a window to display the images from the webcam
    cv::namedWindow("Webcam");

    

    // display the frame until you press a key
    while (1) {
        
        // this will contain the image from the webcam
        cv::Mat frame;

        // capture the next frame from the webcam
        camera >> frame;    

        if (frame.empty()) break; //end of video stream

        // show the image on the window
        cv::imshow("Webcam", frame);
        // wait (10ms) for a key to be pressed
        
        if (cv::waitKey(10) == 27) break; // stop capturing by pressing ESC 

    }
    return 0;
}