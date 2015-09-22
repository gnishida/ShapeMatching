#include "BoundingBox.h"

namespace cga {

BoundingBox::BoundingBox(const std::vector<glm::vec2>& points) {
	minPt.x = (std::numeric_limits<float>::max)();
	minPt.y = (std::numeric_limits<float>::max)();
	minPt.z = 0.0f;
	maxPt.x = -(std::numeric_limits<float>::max)();
	maxPt.y = -(std::numeric_limits<float>::max)();
	maxPt.z = 0.0f;

	for (int i = 0; i < points.size(); ++i) {
		minPt.x = min(minPt.x, points[i].x);
		minPt.y = min(minPt.y, points[i].y);
		maxPt.x = max(maxPt.x, points[i].x);
		maxPt.y = max(maxPt.y, points[i].y);
	}
}

BoundingBox::BoundingBox(const std::vector<glm::vec3>& points) {
	minPt.x = (std::numeric_limits<float>::max)();
	minPt.y = (std::numeric_limits<float>::max)();
	minPt.z = (std::numeric_limits<float>::max)();
	maxPt.x = -(std::numeric_limits<float>::max)();
	maxPt.y = -(std::numeric_limits<float>::max)();
	maxPt.z = -(std::numeric_limits<float>::max)();

	for (int i = 0; i < points.size(); ++i) {
		minPt.x = min(minPt.x, points[i].x);
		minPt.y = min(minPt.y, points[i].y);
		minPt.z = min(minPt.z, points[i].z);
		maxPt.x = max(maxPt.x, points[i].x);
		maxPt.y = max(maxPt.y, points[i].y);
		maxPt.z = max(maxPt.z, points[i].z);
	}
}

}
