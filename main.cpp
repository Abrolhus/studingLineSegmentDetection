#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "img_processing.h"
#include "displayMultipleImages.h"
// extern "C" { // n tava compilando, ai stackOverflow me deu isso aqui pra diferenciar quando o .h e para C, nao para c++
#include "lsd.h"
// }
int main(int argc, char **argv) {
    double* grayMatToArray(cv::Mat img);
    cv::Mat img, hsvImg, grayImg, drawImg;
    cv::Mat canvas;
    // cv::Mat hueImg, saturationImg, lighImg;
    cv::Mat field; //field (remove_background)
    int n; // (LSD: number of line segments found)
    std::vector<cv::Point> endPoints; // end of each line segmnet (LSD)
    std::vector<cv::Point> beginPoints;
    std::string imgPath;


    if(argc >= 3) // ./main input.png outputImg.png
        imgPath = argv[2];
    else // ./main input.png outputImg.png
        imgPath = "img1.jpg";
    img = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
    cv::cvtColor(img, hsvImg, cv::COLOR_RGB2HSV);
    cv::cvtColor(img, grayImg, cv::COLOR_RGB2GRAY);
    drawImg = img.clone();

    remove_background(hsvImg, field); // edited version of the original remove_background, simpler;
    cv::imshow("vai trem", field);
    auto grayValues = grayMatToArray(grayImg);
    auto lineSegs = lsd(&n, grayValues, grayImg.cols, grayImg.rows);

    // storing the begin and end points of the line segments (LSD) in 2 arrays;
    for(int i = 0; i < n; i++){ // for each line segment:
        beginPoints.push_back(cv::Point(lineSegs[7*i + 0], lineSegs[7*i + 1]));
        endPoints.push_back(cv::Point(lineSegs[7*i + 2], lineSegs[7*i + 3]));
    }

    // draws on image (drawImg)
    // cv::cvtColor(drawImg, drawImg, cv::COLOR_BGRA2RGBA);
    for(size_t i = 0; i < endPoints.size(); i++){
        if(field.at<uchar>(beginPoints.at(i)) == 0 && field.at<uchar>(beginPoints.at(i)) == 0)
            continue;
        cv::line(drawImg, beginPoints.at(i), endPoints.at(i), cv::Scalar(240, 0, 0), 2);
        cv::circle(drawImg, endPoints.at(i), 3, cv::Scalar(0, 255, 0), 2);
        cv::circle(drawImg, beginPoints.at(i), 3, cv::Scalar(0, 0, 255), 2);
    }
    for(int y = 0; y < drawImg.rows; y++){
        for(int x = 0; x < drawImg.cols; x++){
            if(field.at<Vec3b>(y, x) == cv::Vec3b(0, 0, 0)){ // field(y,x) == preto
                cv::Vec3b aux = drawImg.at<cv::Vec3b>(y,x);
                auto mean = (int)(aux[0] + aux[1] + aux[2])/3;
                drawImg.at<cv::Vec3b>(y,x) = cv::Vec3b(mean, mean, mean);
                // drawImg.at<cv::Vec3b>(y,x) = cv::Vec3b(0, 0, 0);
                //transforma pixel para escala de cinza;
            }
        }
    }
    std::vector<cv::Mat> aux {img, drawImg};
    canvas = makeCanvas(aux, img.rows, 1);

    // cv::imshow("Agora vai, com elegancia e sofisticacao", drawImg);
    cv::imwrite(imgPath, canvas);
    // cv::waitKey();
}
double* grayMatToArray(cv::Mat img){
    /* Takes a 1-channel matrix, converts it
     * to a 1-dimensional double array.
     * it also stores the number "n" of columns of the original matrix
     */
    auto imgValues = new double[img.rows*img.cols];
    for(int y = 0; y < img.rows; y++){
        for(int x = 0; x < img.cols; x++){
            // double nnnn = img.at<double>(y, x); // note que caso tente img.at<double> vai vir um numero MUITO feio, o programa vai chorar
            imgValues[y*img.cols + x] = (double)img.at<uchar>(y, x);
        }
    }
    return imgValues;
}



