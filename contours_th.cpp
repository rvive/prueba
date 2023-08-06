#include <opencv2/opencv.hpp>
#include <iostream>
// using namespaces to nullify use of cv::function(); syntax and std::function();
using namespace std;
using namespace cv;
 
int main()
{
    // Reading image
    Mat img = imread("/home/vive/latex/images/metod_mocap_m.png"); 
    // Display original image
    imshow("original Image", img);
    //waitKey(0);
 
    // Convert to graycsale
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);

    Mat dst;
    threshold(img_gray,dst,254, 255, THRESH_BINARY);
    imshow("threshold", dst); 
    imwrite("threshold.png", dst);
     
    // Contours detection
    vector<Vec4i> hierarchy;
    vector<vector<Point> > contours;
    findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    Mat drawing_contours = Mat::zeros(dst.size(), CV_8UC3);
    for (int i = 0; i < static_cast<int>(contours.size()); i++)
    {
		  drawContours(drawing_contours, contours, i,Scalar(255,255,255), 1, 8, hierarchy, 0, Point());
    }
    imshow("Contours", drawing_contours);
    imwrite("contours.png", drawing_contours);


    /// Get the moments
    vector<Moments> mu(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        mu[i] = moments(contours[i], false);
    }   

    ///  Get the mass centers:
    vector<Point2f> mc(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
        cout << "\nCentro de masa: " << mc[i].x << ", " << mc[i].y << " ";
    }

    Mat drawing = Mat::zeros(dst.size(), CV_8UC3);
    for (int i = 0; i < static_cast<int>(contours.size()); i++)
    {
      drawContours(drawing, contours, i,Scalar(255,255,255), 1, 8, hierarchy, 0, Point());
      //circle(drawing, mc[i], 1, Scalar(255,255,255), -1, 8, 0);
      line(drawing, mc[i],mc[i],Scalar(255,255,255),1, 8, 0);
    }
    imshow("Center", drawing);
    imwrite("center.png", drawing);

    waitKey(0);
     
    destroyAllWindows();
    return 0;
}
