#include "CGA.h"
#include "GLUtils.h"
#include <map>
#include <iostream>
#include <QDir>
#include "RuleParser.h"

namespace cga {

CGA::CGA() {
}

void CGA::loadRules() {
	// load floor rules
	{
		QDir dir("../cga/floors");
		QStringList filters;
		filters << "*.xml";
		QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
		for (int i = 0; i < fileInfoList.size(); ++i) {
			RuleSet ruleSet;
			parseRules(fileInfoList[i].absoluteFilePath().toUtf8().constData(), ruleSet);

			ruleRepository["floors"].push_back(ruleSet);
		}
	}

	// load window rules
	{
		QDir dir("../cga/windows");
		QStringList filters;
		filters << "*.xml";
		QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);
		for (int i = 0; i < fileInfoList.size(); ++i) {
			RuleSet ruleSet;
			parseRules(fileInfoList[i].absoluteFilePath().toUtf8().constData(), ruleSet);

			ruleRepository["windows"].push_back(ruleSet);
		}
	}
}

void CGA::acceptProposal() {
	ruleSet = proposedRuleSet;
	proposedShapes.clear();
	generate();
}

void CGA::generate() {
	shapes.clear();
	stack.clear();
	stack.push_back(axiom->clone(axiom->_name));

	while (!stack.empty()) {
		boost::shared_ptr<Shape> shape = stack.front();
		stack.pop_front();

		if (ruleSet.contain(shape->_name)) {
			ruleSet.getRule(shape->_name).apply(shape, ruleSet, stack);
		} else {
			if (shape->_name.back() != '!' && shape->_name.back() != '.') {
				//std::cout << "Warning: " << "no rule is found for " << shape->_name << "." << std::endl;
			}
			shapes.push_back(shape);
		}
	}
}

void CGA::generateProposal() {
	proposedShapes.clear();
	stack.clear();
	stack.push_back(axiom->clone(axiom->_name));

	while (!stack.empty()) {
		boost::shared_ptr<Shape> shape = stack.front();
		stack.pop_front();

		if (proposedRuleSet.contain(shape->_name)) {
			proposedRuleSet.getRule(shape->_name).apply(shape, proposedRuleSet, stack);
		} else {
			if (shape->_name.back() != '!' && shape->_name.back() != '.') {
				//std::cout << "Warning: " << "no rule is found for " << shape->_name << "." << std::endl;
			}
			proposedShapes.push_back(shape);
		}
	}
}

void CGA::render(RenderManager* renderManager, bool showScopeCoordinateSystem) {
	renderManager->removeObject("shape");

	for (int i = 0; i < shapes.size(); ++i) {
		shapes[i]->render(renderManager, "shape", 1.0f, showScopeCoordinateSystem);
	}
	for (int i = 0; i < proposedShapes.size(); ++i) {
		proposedShapes[i]->render(renderManager, "shape", 0.2f, showScopeCoordinateSystem);
	}
}

bool CGA::hitFace(const glm::vec3& cameraPos, const glm::vec3& viewDir, Face& face) {
	float min_dist = (std::numeric_limits<float>::max)();
	bool hit = false;

	for (int i = 0; i < shapes.size(); ++i) {
		Face f;
		float dist;
		if (shapes[i]->hitFace(cameraPos, viewDir, f, dist)) {
			if (dist < min_dist) {
				hit = true;
				min_dist = dist;
				face = f;
			}
		}
	}

	return hit;
}

}
