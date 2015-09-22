#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>
#include <boost/shared_ptr.hpp>
#include "Shape.h"

namespace cga {

class RuleSet;

class Value  {
public:
	static enum { TYPE_ABSOLUTE = 0, TYPE_RELATIVE, TYPE_FLOATING };

public:
	int type;
	std::string value;
	bool repeat;

public:
	Value() : type(TYPE_ABSOLUTE), value(""), repeat(false) {}
	Value(int type, const std::string& value, bool repeat = false) : type(type), value(value), repeat(repeat) {}
	
	float getEstimateValue(float size, const RuleSet& ruleSet, const boost::shared_ptr<Shape>& shape) const;
};

class Operator {
public:
	std::string name;

public:
	Operator() {}

	virtual boost::shared_ptr<Shape> apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack) = 0;
};

class Rule {
public:
	std::vector<boost::shared_ptr<Operator> > operators;

public:
	Rule() {}

	void apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack) const;
	static void decodeSplitSizes(float size, const std::vector<Value>& sizes, const std::vector<std::string>& output_names, const RuleSet& ruleSet, const boost::shared_ptr<Shape>& shape, std::vector<float>& decoded_sizes, std::vector<std::string>& decoded_output_names);
};

class RuleSet {
public:
	std::map<std::string, std::string> attrs;
	std::map<std::string, cga::Rule> rules;

public:
	RuleSet() {}

	void clear();
	bool contain(const std::string& name) const;
	Rule getRule(const std::string& name) const { return rules.at(name); }
	Rule& getRule(const std::string& name) { return rules[name]; }
	void addAttr(const std::string& name, const std::string& value);
	void addRule(const std::string& name);
	void addOperator(const std::string& name, const boost::shared_ptr<Operator>& op);
	float evalFloat(const std::string& attr_name, const boost::shared_ptr<Shape>& shape) const;
	std::string evalString(const std::string& attr_name, const boost::shared_ptr<Shape>& shape) const;

	void merge(const RuleSet& ruleSet);
};

}
