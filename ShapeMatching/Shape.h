#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <map>
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include "Face.h"
#include "Stroke.h"

class RenderManager;

namespace cga {

class CGA;
class RuleSet;

class Shape {
public:
	static float explode_factor;

public:
	std::string _name;
	bool _removed;
	glm::mat4 _modelMat;
	glm::vec3 _color;
	bool _textureEnabled;
	std::string _texture;
	std::vector<glm::vec2> _texCoords;
	glm::vec3 _scope;
	glm::vec3 _prev_scope;
	glm::mat4 _pivot;

public:
	virtual boost::shared_ptr<Shape> clone(const std::string& name) const;
	virtual void comp(const std::map<std::string, std::string>& name_map, std::vector<boost::shared_ptr<Shape> >& shapes);
	virtual boost::shared_ptr<Shape> extrude(const std::string& name, float height);
	void nil();
	virtual void split(int splitAxis, const std::vector<float>& sizes, const std::vector<std::string>& names, std::vector<boost::shared_ptr<Shape> >& objects);
	virtual void render(RenderManager* renderManager, const std::string& name, float opacity, bool showScopeCoordinateSystem) const;

	virtual bool hitFace(const glm::vec3& cameraPos, const glm::vec3& viewDir, Face& face, float& dist);
	virtual void findRule(const std::vector<Stroke>& strokes, int sketch_step, CGA* cga);

protected:
	void drawAxes(RenderManager* renderManager, const glm::mat4& modelMat) const;
};

}
