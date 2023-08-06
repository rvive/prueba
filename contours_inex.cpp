#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);


/// Function header
void thresh_callback(int, void*);

/** @function main */
int main(int argc, char** argv)
{
    src = imread("/home/vive/ya.png"); 

    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));

    const char* source_window = "Source";
    namedWindow(source_window, WINDOW_AUTOSIZE);
    imshow(source_window, src);

    createTrackbar(" Canny thresh:", "Source", &thresh, max_thresh, thresh_callback);
    thresh_callback(0, 0);

    waitKey(0);
    return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void*)
{
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Rect rect;
    Point pt1, pt2;

    /// Detect edges using canny
    Canny(src_gray, canny_output, thresh, thresh * 2, 3);
    /// Find contours
    //findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));


    /// Draw contours
    Mat drawing_in = Mat::zeros(canny_output.size(), CV_8UC3);
    Mat drawing_ex = Mat::zeros(canny_output.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++)
    {
        drawContours(drawing_in, contours, i,Scalar(255,255,255), 1, 8, hierarchy, 0, Point());
				//drawContours(drawing_ex, contours[1], i,Scalar(0,0,255), 1, 8, hierarchy, 0, Point());
        //imwrite("/home/vive/contours.png",drawing);
    }

    /// Show in a window
    imshow("Contours in", drawing_in);
    imshow("Contours ex", drawing_ex);

}

