#pragma once

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

namespace cvutils {

double mat_get_value(const cv::Mat& m, int r, int c);
void mat_set_value(cv::Mat& m, int r, int c, double val);
double mat_sum(const cv::Mat& m);
cv::Rect computeBoundingBoxFromImage(const cv::Mat& img);

}
