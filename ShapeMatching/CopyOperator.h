#pragma once

#include "Rule.h"

namespace cga {

class CopyOperator : public Operator {
private:
	std::string copy_name;

public:
	CopyOperator(const std::string& copy_name);

	boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack);
};

}