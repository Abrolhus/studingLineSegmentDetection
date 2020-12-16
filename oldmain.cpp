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


// extern "C" { // n tava compilando, ai stackOverflow me deu isso aqui pra diferenciar quando o .h e para C, nao para c++
#include "lsd.h"
// #define VIDEO
int main(int argc, char **argv)
{
    cv::Mat img, grayImg;
    int n;
	img = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    std::string imgPath = argv[2]; // for instance "folder1/image171.jpg"
    cv::Mat binarizada(img.rows, img.cols, CV_8UC3, cv::Scalar(0,0,0));
    cv::Mat field, auxImg;
    auxImg = img.clone();
    remove_background(img, field);
    cv::imshow("vai ", field);
    cv::waitKey();

    remove_background(auxImg, img); // remove_background(img, img) n'ao funciona como esperado, ja que no comeco da funcao a segunda matriz eh zerada. na real nao sei dizer exatamente, melhor testar....
    cv::cvtColor(img, grayImg, cv::COLOR_RGBA2GRAY);
    std::vector<cv::Vec4i> lines; // vetor de linhas, sendo cada linha {p1y, p1x, p2y, p2x}
    double* grayValues = new double[grayImg.rows*grayImg.cols];
    double* lineSegs;
    std::vector<cv::Point> endPoints;
    std::vector<cv::Point> beginPoints;
    for(int y = 0; y < grayImg.rows; y++){
        for(int x = 0; x < grayImg.cols; x++){
            //armazena os valores de cor de cada pixel num array UNIDIMENSIONAL de doubles;
            // double nnnn = grayImg.at<double>(y, x); // note que caso tente img.at<double> vai vir um numero MUITO feio, o programa vai chorar
            double atxy = (double)grayImg.at<uchar>(y, x);
            grayValues[y*grayImg.cols + x] = atxy;
        }
    }
    // std::cout << "dimens da imagem: " << grayImg.rows << " " << grayImg.cols;
    lineSegs = lsd(&n, grayValues, grayImg.cols, grayImg.rows);
    /*
    @return            A double array of size 7 x n_out, containing the list

  7                        of line segments detected. The array contains first
  6                        7 values of line segment number 1, then the 7 values
  5                        of line segment number 2, and so on, and it finish
  4                        by the 7 values of line segment number n_out.
  3                        The seven values are:
  2                        - x1,y1,x2,y2,width,p,-log10(NFA)
  1                        .
271                        for a line segment from coordinates (x1,y1) to (x2,y2),
  1                        a width 'width', an angle precision of p in (0,1) given
  2                        by angle_tolerance/180 degree, and NFA value 'NFA'.
  3                        If 'out' is the returned pointer, the 7 values of
  4                        line segment number 'n+1' are obtained with
  5                        'out[7n+0]' to 'out[7n+6]'.
  6  */
    for(int i=0;i<n;i++)
    {
        beginPoints.push_back(cv::Point(lineSegs[7*i + 0], lineSegs[7*i + 1]));
        endPoints.push_back(cv::Point(lineSegs[7*i + 2], lineSegs[7*i + 3]));


            //printf("%f ",lineSegs[7*i+j]);
        double angle = 0;
        cv::line(img, cv::Point(lineSegs[7*i+0], lineSegs[7*i+1]), cv::Point(lineSegs[7*i+2], lineSegs[7*i+3]), cv::Scalar(234, 100, 0), 2);
        cv::line(binarizada, cv::Point(lineSegs[7*i+0], lineSegs[7*i+1]), cv::Point(lineSegs[7*i+2], lineSegs[7*i+3]), cv::Scalar(255, 255, 255), 2);
        lines.push_back({(int)lineSegs[7*i+1],(int)lineSegs[7*i+0], (int)lineSegs[7*i+3], (int)lineSegs[7*i+2]});

    }

    for(int i =0; i < endPoints.size(); i++){
        cv::circle(img, endPoints.at(i), 7, cv::Scalar(0, 255, 0), 3);
        cv::circle(img, beginPoints.at(i), 7, cv::Scalar(0, 0, 255), 3); }
    cout << "begin:" << beginPoints.size() <<  " == " <<  "end: " << endPoints.size() << "\n";

    std::vector<field_point> result_intersections;
    std::vector<goalposts> goalPosts;

    std::map<int, cv::Scalar> featureToColor = {
        {0, cv::Scalar(0,0,155)},
        {1, cv::Scalar(0,155,155)},
        {2, cv::Scalar(155,0,155)},
        {3, cv::Scalar(50,100,155)},
        {4, cv::Scalar(55,20,155)}

    };
	vector<Vec4i> ellipse_prob_lines;
    cv::imwrite(imgPath, img);
    return 0;
}
