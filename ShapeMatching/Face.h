#pragma once

#include <boost/shared_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

namespace cga {

class Shape;

class Face {
public:
	Shape* shape;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;
	glm::vec3 color;
	std::vector<glm::vec2> texCoords;

public:
	Face() {}
	Face(Shape* shape, const std::vector<glm::vec3>& points, const glm::vec3& normal, const glm::vec3& color, const std::vector<glm::vec2>& texCoords);
	Face(Shape* shape, const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& normals, const glm::vec3& color, const std::vector<glm::vec2>& texCoords);
};

}
