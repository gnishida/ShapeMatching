#pragma once

#include <boost/shared_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include "Shape.h"

class RenderManager;

namespace cga {

class RuleSet;

class Rectangle : public Shape {
public:
	Rectangle() {}
	Rectangle(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, float width, float height, const glm::vec3& color);
	Rectangle(const std::string& name, const glm::mat4& pivot, const glm::mat4& modelMat, float width, float height, const glm::vec3& color, const std::string& texture, float u1, float v1, float u2, float v2);
	boost::shared_ptr<Shape> clone(const std::string& name) const;
	boost::shared_ptr<Shape> extrude(const std::string& name, float height);
	boost::shared_ptr<Shape> offset(const std::string& name, float offsetDistance, int offsetSelector);
	void split(int splitAxis, const std::vector<float>& ratios, const std::vector<std::string>& names, std::vector<boost::shared_ptr<Shape> >& objects);
	void render(RenderManager* renderManager, const std::string& name, float opacity, bool showScopeCoordinateSystem) const;
	bool hitFace(const glm::vec3& cameraPos, const glm::vec3& viewDir, Face& face, float& dist);
	void findRule(const std::vector<Stroke>& strokes, int sketch_step, CGA* cga);
};

}

