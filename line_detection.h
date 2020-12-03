#ifndef LINE_DETECTION_H
#define LINE_DETECTION_H

#include <iostream>
#include <math.h>
//#include <cv.h>
//#include <highgui.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

struct point_dis
{
	Point pnt;
	double distance;
};

void line_extraction(Mat image, vector<Vec4i> &produced_lines, int hor_step, int ver_step);

#endif
