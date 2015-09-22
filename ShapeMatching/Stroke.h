#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

class Stroke {
public:
	std::vector<glm::vec2> points;
	std::vector<glm::vec3> points3d;

public:
	Stroke() {}
	Stroke(const glm::vec2& point) { points.push_back(point); }

	void clear();
	float length();
};
