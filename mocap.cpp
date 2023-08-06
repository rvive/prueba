#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

float posArray[30] = {};
float posArrayPrev[30] = {};
float dArray[10] = {};
float disArray[10] = {};
float velArray[10] = {};
float aceArray[10] = {};

ofstream dataPos("/home/vive/pruebas/pruebas_mocap_m1.dat");
int main()
{   
    // --------------------ADQUSICIÓN DE IMAGEN----------------------
    Mat img = imread("/home/vive/images_mocap/escuadra/visor_ir.jpg", IMREAD_GRAYSCALE); 
    imshow("original Image", img);
    waitKey(0);

    // -----------------------PROCESAMIENTO--------------------------
    // Thresholding
    Mat dst;
    threshold(img,dst,254, 255, THRESH_BINARY);
    imshow("threshold", dst);
    waitKey(0);
    // Contours detection
    vector<Vec4i> hierarchy;
    vector<vector<Point> > contours;
    findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
    // Draw contours
    Mat drawing_contours = Mat::zeros(dst.size(), CV_8UC3);
    for (int i = 0; i < static_cast<int>(contours.size()); i++)
    {
		  drawContours(drawing_contours, contours, i,Scalar(255,0,0), 1, 8, hierarchy, 0, Point());
    }
    imshow("Contours", drawing_contours);


    // --------------------OBTENCIÓN DE PARÁMETROS------------------
    float sumx, sumy;
    int x, y, n_points, k;
    float distance, dx, dy, d, vel, vel_prev, delta_vel, a;
    k = 0;
    for (int i = 0; i < static_cast<int>(contours.size()); i++)
    {
        sumx = 0.0;
        sumy = 0.0;
        n_points = 0;
        std::cout << "------------- Marcador " << i + 1 << " -------------\n";
        for (int j = 0; j < static_cast<int>(contours[i].size()) ; j++)
        {
            x = contours[i][j].x;
            y = contours[i][j].y;

            if(y > 0)
            { 
                n_points = n_points + 1;
                sumx = sumx + x;
                sumy = sumy + y;
            }
        }
        k = 2*i;
        posArray[k] = sumx/n_points;
        posArray[k+1] = sumy/n_points;

        distance = sqrt(pow(posArray[k], 2) + pow(posArray[k+1],2) );
        dx = posArray[k]-posArrayPrev[k];
        dy = posArray[k+1]-posArrayPrev[k+1];

        d = sqrt(pow(dx, 2) + pow(dy,2)); // Desplazamiento del marcador 

        dataPos << std::fixed << posArray[k] << "," << posArray[k+1] << "," << d << "," << i+1<< endl;
    }


    waitKey(0);
    destroyAllWindows();
    return 0;
}
