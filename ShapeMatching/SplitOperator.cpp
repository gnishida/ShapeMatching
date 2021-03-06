#include "SplitOperator.h"
#include "CGA.h"
#include "Shape.h"

namespace cga {

SplitOperator::SplitOperator(int splitAxis, const std::vector<Value>& sizes, const std::vector<std::string>& output_names) {
	this->name = "split";
	this->splitAxis = splitAxis;
	this->sizes = sizes;
	this->output_names = output_names;
}

boost::shared_ptr<Shape> SplitOperator::apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack) {
	std::vector<boost::shared_ptr<Shape> > floors;

	std::vector<float> decoded_sizes;
	std::vector<std::string> decoded_output_names;
	if (splitAxis == DIRECTION_X) {
		Rule::decodeSplitSizes(shape->_scope.x, sizes, output_names, ruleSet, shape, decoded_sizes, decoded_output_names);
	} else if (splitAxis == DIRECTION_Y) {
		Rule::decodeSplitSizes(shape->_scope.y, sizes, output_names, ruleSet, shape, decoded_sizes, decoded_output_names);
	} else if (splitAxis == DIRECTION_Z) {
		Rule::decodeSplitSizes(shape->_scope.z, sizes, output_names, ruleSet, shape, decoded_sizes, decoded_output_names);
	}

	shape->split(splitAxis, decoded_sizes, decoded_output_names, floors);
	stack.insert(stack.end(), floors.begin(), floors.end());

	//delete shape;
	//return NULL;
	return boost::shared_ptr<Shape>();
}

}
