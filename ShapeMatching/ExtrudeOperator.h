#pragma once

#include "Rule.h"

namespace cga {

class ExtrudeOperator : public Operator {
private:
	std::string height;

public:
	ExtrudeOperator(const std::string& height);

	boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack);
};

}