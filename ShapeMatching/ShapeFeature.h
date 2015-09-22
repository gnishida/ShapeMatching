#pragma once

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <map>
#include <string>

class ShapeFeature {
public:
	float pitch_angle;
	float yaw_angle;
	cv::Mat image;
	std::string cga_filename;
	std::map<std::string, std::string> attrs;

public:
	ShapeFeature() {}
	ShapeFeature(float pitch_angle, float yaw_angle, const cv::Mat& image, const std::string& cga_filename, const std::map<std::string, std::string>& attrs) : pitch_angle(pitch_angle), yaw_angle(yaw_angle), image(image), cga_filename(cga_filename), attrs(attrs) {}
};

