#include "Stroke.h"

void Stroke::clear() {
	points.clear();
	points3d.clear();
}

float Stroke::length() {
	float len = 0.0f;
	for (int i = 0; i < points.size() - 1; ++i) {
		len += glm::length(points[i + 1] - points[i]);
	}
	return len;
}