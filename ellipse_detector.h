#ifndef ELLIPSE_DETECTOR_H
#define ELLIPSE_DETECTOR_H

#include <iostream>
#include <math.h>
#include <time.h>
//#include <cv.h>
//#include <highgui.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#define CONNECTION_ANGLE_THRES 30
#define CONNECTION_THRES 10

using namespace cv;

void detect_ellipse(Mat image, vector<Vec4i> lines, vector<Vec4i> &ellipse_prob_lines);

#endif