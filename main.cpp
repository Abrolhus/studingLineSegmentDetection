#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "goal_detection.h"
#include "geometry_utils.h"
#include "img_processing.h"
#include "line_detection.h"
#include "line_feature_detection.h"
#include "feature_extraction.h"
#include "dis_ang_translation.h"
#include "displayMultipleImages.h"


// extern "C" { // n tava compilando, ai stackOverflow me deu isso aqui pra diferenciar quando o .h e para C, nao para c++
#include "lsd.h"
// #define VIDEO
int main(int argc, char **argv)
{
    cv::Mat img, grayImg;
    std::vector<cv::Mat> imagens;
    std::vector<cv::Mat> binarizadas;
    cv::Mat canvas;
    for(int i = 1; i < argc; i++){
        imagens.push_back(cv::imread(argv[i]));
    }
    for(int i = 1; i < argc; i++){
        cv::Mat auxImg;
        cv::cvtColor(imagens[i], auxImg, cv::COLOR_RGBA2GRAY);
        binarizadas.push_back(auxImg.clone());
    }
    int n;
    double* grayValues = new double[grayImg.rows*grayImg.cols];
    double* lineSegs;
/*
  for(x=0;x<X;x++)
    for(y=0;y<Y;y++)
      image[x+y*X] = x<X/2 ? 0.0 : 64.0; // image(x,y)
    */


    for(int y = 0; y < grayImg.rows; y++){
        for(int x = 0; x < grayImg.cols; x++){
            //armazena os valores de cor de cada pixel num array UNIDIMENSIONAL de doubles;
            //
            // std::cout << y*grayImg.cols + x << " ";
            // double nnnn = grayImg.at<double>(y, x); // note que caso tente img.at<double> vai vir um numero MUITO feio, o programa vai chorar
            double atxy = (double)grayImg.at<uchar>(y, x);
            grayValues[y*grayImg.cols + x] = atxy;
        }
    }
    std::cout << "dimens da imagem: " << grayImg.rows << " " << grayImg.cols;
    //lineSegs = lsd(&n, grayValues, grayImg.cols, grayImg.rows);

    lineSegs = lsd(&n, grayValues, grayImg.cols, grayImg.rows);
    /* print output */
    // printf("%d line segments found:\n",n);
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<7;j++);
            //printf("%f ",lineSegs[7*i+j]);
        double angle = 0;
        cv::line(img, cv::Point(lineSegs[7*i+0], lineSegs[7*i+1]), cv::Point(lineSegs[7*i+2], lineSegs[7*i+3]), cv::Scalar(234, 100, 0), 2);
        cv::line(binarizada, cv::Point(lineSegs[7*i+0], lineSegs[7*i+1]), cv::Point(lineSegs[7*i+2], lineSegs[7*i+3]), cv::Scalar(255, 255, 255), 2);
    }
    std::vector<field_point> result_intersections;
    std::vector<goalposts> goalPosts;

    std::map<int, cv::Scalar> featureToColor = {
        {0, cv::Scalar(0,0,155)},
        {1, cv::Scalar(0,155,155)},
        {2, cv::Scalar(155,0,155)},
        {3, cv::Scalar(50,100,155)},
        {4, cv::Scalar(55,20,155)}

    };
    cv::putText(img, "LLLLL", Point(img.cols*2/3, img.rows*2/3 + 00),cv::FONT_HERSHEY_DUPLEX, 1.0,  featureToColor.at(0));
    cv::putText(img, "TTTTT", Point(img.cols*2/3, img.rows*2/3 + 20),cv::FONT_HERSHEY_DUPLEX, 1.0,  featureToColor.at(1));
    cv::putText(img, ",,,,,", Point(img.cols*2/3, img.rows*2/3 + 40),cv::FONT_HERSHEY_DUPLEX, 1.0,  featureToColor.at(2));
    cv::putText(img, "XXXXX", Point(img.cols*2/3, img.rows*2/3 + 60),cv::FONT_HERSHEY_DUPLEX, 1.0,  featureToColor.at(3));
    cv::putText(img, ".....", Point(img.cols*2/3, img.rows*2/3 + 80),cv::FONT_HERSHEY_DUPLEX, 1.0,  featureToColor.at(4));

//
    std::vector<cv::Vec4i> lines;
    cout << "line_extraction ";
	line_extraction(binarizada, lines, 5, 5);
    // cout << lines.size() << " fkjadslk;fjlasjdf ";
	vector<Vec4i> ellipse_prob_lines;
    cout << "detect_ellipse ";
	// detect_ellipse(img_lines_binary, lines, ellipse_prob_lines);

    cout << "line_most_prob_features ";
	line_most_prob_features(binarizada, lines, ellipse_prob_lines,
							result_intersections);

    for(auto& e : result_intersections){
        cout << "uai";
        circle(binarizada, Point(e.position.x, e.position.y), 3, featureToColor.at(e.type), 3, 8, 0);
    }

    canvas = makeCanvas(imagens, 720, 3);
    cv::imshow("imagens!!!!!!", canvas);

    cv::imshow("vai tio", img);
    cv::imshow("tb vai tio", binarizada);
    cv::waitKey(0);
    return 0;
}
