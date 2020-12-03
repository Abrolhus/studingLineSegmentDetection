#include <iostream>
#include <math.h>
#include <time.h>
//#include <cv.h>
//#include <highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "img_processing.h"
#include "line_detection.h"
#include "line_feature_detection.h"
#include "ellipse_detector.h"
#include "goal_detection.h"
#include "feature_extraction.h"

using namespace cv;
using namespace std;

void extract_features(Mat img_rgb, vector<field_point> &result_intersections,
					  vector<goalposts> &goalPosts)
{
	Mat img_hsv;
	Mat img_lines_binary, img_posts_binary, img_ball_binary;

    cout << "color ";
	cvtColor(img_rgb, img_hsv, CV_BGR2HSV);

	vector<Point> goalRoots;
	double hor_hist[img_hsv.cols];
	int ver_hist[img_hsv.rows];
    cout << "remove_background ";
	remove_background(img_hsv, img_lines_binary, img_posts_binary,
					  img_ball_binary, goalRoots, hor_hist, ver_hist);

	vector<Vec4i> lines;
    cout << "line_extraction ";
	line_extraction(img_lines_binary, lines, 5, 5);
	vector<Vec4i> ellipse_prob_lines;
    cout << "detect_ellipse ";
	// detect_ellipse(img_lines_binary, lines, ellipse_prob_lines);

    cout << "line_most_prob_features ";
	line_most_prob_features(img_lines_binary, lines, ellipse_prob_lines,
							result_intersections);

    cout << "goalPostDetection ";
	// goalPostDetection(img_posts_binary, goalRoots, hor_hist, ver_hist,
					  // goalPosts);

	return;
}
