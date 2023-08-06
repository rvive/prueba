#include <opencv2/opencv.hpp>
#include <iostream>
// using namespaces to nullify use of cv::function(); syntax and std::function();
using namespace std;
using namespace cv;
 
int main()
{
    // Reading image
    Mat img = imread("/home/vive/ya.png");
    // Display original image
    imshow("original Image", img);
    cout << img << endl;

    // Canny edge detection.
    Mat edges;
    Canny(img, edges, 250, 255*3,3);
    //std::cout << "PRIMER VALOR DE EDGES: " << edges.at((int) 0) << "\n";
    // Display canny edge detected image
    imshow("Canny edge detection", edges);
    waitKey(0);
      
    destroyAllWindows();
    return 0;
}

