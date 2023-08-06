#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // Cargar imagen
    Mat image = imread("/home/vive/c++/mario_luigi.jpg");
    Mat ref = imread("/home/vive/c++/luigi.png");
    

    // Invertir colores de la imagen
    Mat inverted;
    bitwise_not(image, inverted);

    // Convertir a escala de grises
    Mat gray;
    cvtColor(inverted, gray, COLOR_BGR2GRAY);


    // Aplicar umbral
    Mat thresh;
    threshold(gray, thresh, 100, 255, THRESH_BINARY);

    // Encontrar contornos
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(thresh, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	
    // Dibujar contornos y bounding box
    Mat silueta = Mat::zeros(image.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        drawContours(silueta, contours, (int)i, Scalar(255,0,0), 1, LINE_8, hierarchy, 0);
        Rect boundingBox = boundingRect(contours[i]);
    		rectangle(silueta, boundingBox, Scalar(255,40,155), 2);
    }
		imshow("silueta", silueta);
		
		// Crear vector de matrices para almacenar subimágenes
		vector<Mat> subImages;

		// Dibujar bounding box en input image y recortar subimagen correspondiente
		for (int i = 0; i < static_cast<int>(contours.size()); i++)
		{ 
			Rect boundingBox = boundingRect(contours[i]);
			rectangle(image, boundingBox, CV_RGB(0,0,0), 2);
			Mat subImage = image(boundingBox);


		// Almacenar subimagen en el vector de matrices
			subImages.push_back(subImage);
		}

		// Mostrar imagen original con bounding boxes dibujados
		imshow("image", image);
		imshow("image_referencia", ref);

		// Mostrar cada subimagen en una ventana diferente
		for (int i = 0; i < subImages.size(); i++)
		{
			imshow("sub image " + to_string(i + 1), subImages[i]);
		}
		
		
		
		
		
		// HISTOGRAMA DE IMAGEN DE REFERENCIA
		vector<Mat> bgr_planes_ref;
	  split( ref, bgr_planes_ref );
	  
	  int histSize = 256;
	  float range[] = { 0, 256 }; 
	  const float* histRange = { range };
	  bool uniform = true, accumulate = false;
	  
	  Mat b_hist, g_hist, r_hist;
	  calcHist( &bgr_planes_ref[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
	  calcHist( &bgr_planes_ref[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
	  calcHist( &bgr_planes_ref[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
	  
	  int hist_w = 512, hist_h = 400;
	  int bin_w = cvRound( (double) hist_w/histSize );
	  Mat histImage_ref( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) ); 
	  
	  normalize(b_hist, b_hist, 0, histImage_ref.rows, NORM_MINMAX, -1, Mat() );
	  normalize(g_hist, g_hist, 0, histImage_ref.rows, NORM_MINMAX, -1, Mat() );
	  normalize(r_hist, r_hist, 0, histImage_ref.rows, NORM_MINMAX, -1, Mat() );
	  
	  for( int i = 1; i < histSize; i++ )
	  {
	      line( histImage_ref, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
	            Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
	            Scalar( 255, 0, 0), 2, 8, 0  );
	      line( histImage_ref, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
	            Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
	            Scalar( 0, 255, 0), 2, 8, 0  );
	      line( histImage_ref, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
	            Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
	            Scalar( 0, 0, 255), 2, 8, 0  );
	  }

	  imshow("Histograma de referencia", histImage_ref);
	  
	  
	  
	  

		// HISTOGRAMA POR CADA SUBIMAGEN 
		std::vector<cv::Mat> vec_his;
		std::vector<double> vec_corre;


		for (int i = 0; i < subImages.size(); i++)
		{
		  vector<Mat> bgr_planes;
		  split( subImages[i], bgr_planes );
		  
		  int histSize = 256;
		  float range[] = { 0, 256 }; //the upper boundary is exclusive
		  //float range[] = { 0, 256 }; //Todo el RGB 
		  const float* histRange = { range };
		  bool uniform = true, accumulate = false;
		  
		  Mat b_hist, g_hist, r_hist;
		  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
		  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
		  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
		  
		  int hist_w = 512, hist_h = 400;
		  int bin_w = cvRound( (double) hist_w/histSize );
		  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) ); 
		  
		  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
		  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
		  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
		  
		  for( int i = 1; i < histSize; i++ )
		  {
		      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
		            Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
		            Scalar( 255, 0, 0), 2, 8, 0  );
		      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
		            Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
		            Scalar( 0, 255, 0), 2, 8, 0  );
		      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
		            Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
		            Scalar( 0, 0, 255), 2, 8, 0  );
		  }

			vec_his.push_back(histImage);
			
		  imshow("calcHist" + to_string(i+1), histImage);

		}
		for( int i = 0; i < static_cast<int>(vec_his.size()) ; i++)
		{
				histImage_ref.convertTo(histImage_ref, CV_32F);
				vec_his[i].convertTo(vec_his[i], CV_32F);
				//vec_his[1].convertTo(vec_his[1], CV_32F);
				// Calcular la similitud entre el histograma de referencia y los histogramas de las imágenes
				//double similitud_1 = compareHist(histImage_ref, vec_his[0], cv::HISTCMP_CORREL);
				vec_corre.push_back(compareHist(histImage_ref, vec_his[i], cv::HISTCMP_CORREL));
		}
	

	double max = 0;
	for (int i=0 ; i < static_cast<int>(vec_corre.size()) ; i++)
	{
		max = vec_corre[0];
		if (vec_corre[i] > max)
		{
			max = vec_corre[i];
		} 
		
	}
	
	cout << "maximo: " << max << endl;
				

    
		waitKey(0);
    return 0;
}




