#ifndef DISPLAYMULTIPLEIMAGES_H
#define DISPLAYMULTIPLEIMAGES_H

#include <opencv2/opencv.hpp>
#include <iostream>
// #include <stdio.h>
#include <stdarg.h>

cv::Mat makeCanvas(std::vector<cv::Mat>& vecMat, int windowHeight, int nRows);

#endif

