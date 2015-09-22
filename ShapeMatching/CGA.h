#pragma once

#include "RenderManager.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Vertex.h"
#include "Rule.h"
#include "Shape.h"
#include "Face.h"
#include <map>

namespace cga {

enum { DIRECTION_X = 0, DIRECTION_Y, DIRECTION_Z, SCOPE_SX, SCOPE_SY };
enum { REVOLVE_X = 0, REVOLVE_Y };
enum { MODE_ABSOLUTE = 0, MODE_RELATIVE };
enum { COORD_SYSTEM_WORLD = 0, COORD_SYSTEM_OBJECT };
enum { AXES_SCOPE_XY = 0, AXES_SCOPE_XZ, AXES_SCOPE_YX, AXES_SCOPE_YZ, AXES_SCOPE_ZX, AXES_SCOPE_ZY, AXES_WORLD_XY, AXES_WORLD_XZ, AXES_WORLD_YX, AXES_WORLD_YZ, AXES_WORLD_ZX, AXES_WORLD_ZY };
enum { SELECTOR_ALL = 0, SELECTOR_INSIDE, SELECTOR_BORDER };
enum { AXES_SELECTOR_XYZ = 0, AXES_SELECTOR_X, AXES_SELECTOR_Y, AXES_SELECTOR_Z, AXES_SELECTOR_XY, AXES_SELECTOR_XZ, AXES_SELECTOR_YZ };


enum { STEP_FLOOR = 0, STEP_WINDOW };


const float M_PI = 3.1415926f;

class CGA {
public:
	glm::mat4 modelMat;
	boost::shared_ptr<Shape> axiom;
	std::list<boost::shared_ptr<Shape> > stack;
	std::vector<boost::shared_ptr<Shape> > shapes;
	std::vector<boost::shared_ptr<Shape> > proposedShapes;

	RuleSet ruleSet;
	RuleSet proposedRuleSet;

	std::map<std::string, std::vector<RuleSet> > ruleRepository;

public:
	CGA();

	void loadRules();
	void acceptProposal();
	void generate();
	void generateProposal();
	void render(RenderManager* renderManager, bool showScopeCoordinateSystem = false);

	bool hitFace(const glm::vec3& cameraPos, const glm::vec3& viewDir, Face& face);
};

}
