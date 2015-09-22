#include "Shape.h"
#include "GLUtils.h"
#include <iostream>
#include <sstream>
#include "CGA.h"
#include "BoundingBox.h"

namespace cga {

float Shape::explode_factor = 1.01f;

boost::shared_ptr<Shape> Shape::clone(const std::string& name) const {
	throw "clone() is not supported.";
}

void Shape::comp(const std::map<std::string, std::string>& name_map, std::vector<boost::shared_ptr<Shape> >& shapes) {
	throw "comp() is not supported.";
}

boost::shared_ptr<Shape> Shape::extrude(const std::string& name, float height) {
	throw "extrude() is not supported.";
}

void Shape::nil() {
	_removed = true;
}

void Shape::split(int splitAxis, const std::vector<float>& sizes, const std::vector<std::string>& names, std::vector<boost::shared_ptr<Shape> >& objects) {
	throw "split() is not supported.";
}

void Shape::render(RenderManager* renderManager, const std::string& name, float opacity, bool showScopeCoordinateSystem) const {
	throw "render() is not supported.";
}

bool Shape::hitFace(const glm::vec3& cameraPos, const glm::vec3& viewDir, Face& face, float& dist) {
	throw "hitFace() is not supported.";
}

void Shape::findRule(const std::vector<Stroke>& strokes, int sketch_step, CGA* cga) {
	throw "findRule() is not supported.";
}

void Shape::drawAxes(RenderManager* renderManager, const glm::mat4& modelMat) const {
	std::vector<Vertex> vertices;
	glutils::drawAxes(0.1, 3, modelMat, vertices);
	renderManager->addObject("axis", "", vertices);
}

}