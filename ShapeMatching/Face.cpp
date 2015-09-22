#include "Face.h"

namespace cga {

Face::Face(Shape* shape, const std::vector<glm::vec3>& points, const glm::vec3& normal, const glm::vec3& color, const std::vector<glm::vec2>& texCoords) {
	this->shape = shape;
	this->points = points;
	for (int i = 0; i < points.size(); ++i) {
		this->normals.push_back(normal);
	}
	this->color = color;
	this->texCoords = texCoords;
}

Face::Face(Shape* shape, const std::vector<glm::vec3>& points, const std::vector<glm::vec3>& normals, const glm::vec3& color, const std::vector<glm::vec2>& texCoords) {
	this->shape = shape;
	this->points = points;
	this->normals = normals;
	this->color = color;
	this->texCoords = texCoords;
}

}