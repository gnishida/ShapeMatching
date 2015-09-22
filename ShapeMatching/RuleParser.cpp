#include "RuleParser.h"
#include "CompOperator.h"
#include "CopyOperator.h"
#include "ExtrudeOperator.h"
#include "SplitOperator.h"
#include "CGA.h"
#include <iostream>

namespace cga {

void parseRules(const std::string& filename, RuleSet& ruleSet) {
	ruleSet.clear();

	QFile file(filename.c_str());

	QDomDocument doc;
	doc.setContent(&file, true);
	QDomElement root = doc.documentElement();

	QDomNode child_node = root.firstChild();
	while (!child_node.isNull()) {
		if (child_node.toElement().tagName() == "attr") {
			if (!child_node.toElement().hasAttribute("name")) {
				std::cout << "<attr> tag must contain name attribute." << std::endl;
				throw "<attr> tag must contain name attribute.";
			}
			std::string name = child_node.toElement().attribute("name").toUtf8().constData();

			if (!child_node.toElement().hasAttribute("value")) {
				std::cout << "<attr> tag must contain value attribute." << std::endl;
				throw "<attr> tag must contain value attribute.";
			}
			std::string value = child_node.toElement().attribute("value").toUtf8().constData();

			ruleSet.addAttr(name, value);
		} else if (child_node.toElement().tagName() == "rule") {
			if (!child_node.toElement().hasAttribute("name")) {
				std::cout << "<rule> tag must contain name attribute." << std::endl;
				throw "<rule> tag must contain name attribute.";
			}
			std::string name = child_node.toElement().attribute("name").toUtf8().constData();

			ruleSet.addRule(name);
			parseRuleNode(child_node.toElement(), ruleSet.rules[name]);
		}

		child_node = child_node.nextSibling();
	}
}

void parseRule(const std::string& filename, std::vector<Rule>& rules) {
	QFile file(filename.c_str());

	QDomDocument doc;
	doc.setContent(&file, true);
	QDomElement root = doc.documentElement();

	QDomNode child_node = root.firstChild();
	while (!child_node.isNull()) {
		if (child_node.toElement().tagName() == "rule") {
			Rule rule;
			parseRuleNode(root, rule);

			rules.push_back(rule);
		}

		child_node = child_node.nextSibling();
	}
}

void parseRuleNode(const QDomElement& rule_node, Rule& rule) {
	rule.operators.clear();

	QDomNode operator_node = rule_node.firstChild();
	while (!operator_node.isNull()) {
		std::string operator_name = operator_node.toElement().tagName().toUtf8().constData();

		if (operator_name == "comp") {
			rule.operators.push_back(parseCompOperator(operator_node));
		} else if (operator_name == "copy") {
			rule.operators.push_back(parseCopyOperator(operator_node));
		} else if (operator_name == "extrude") {
			rule.operators.push_back(parseExtrudeOperator(operator_node));
		} else if (operator_name == "split") {
			rule.operators.push_back(parseSplitOperator(operator_node));
		}

		operator_node = operator_node.nextSibling();
	}
}

boost::shared_ptr<Operator> parseCompOperator(const QDomNode& node) {
	std::string front_name;
	std::string side_name;
	std::string top_name;
	std::string bottom_name;
	std::string inside_name;
	std::string border_name;
	std::string vertical_name;
	std::map<std::string, std::string> name_map;

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString name = child.toElement().attribute("name");
			std::string value = child.toElement().attribute("value").toUtf8().constData();

			if (name == "front") {
				name_map["front"] = value;
			} else if (name == "right") {
				name_map["right"] = value;
			} else if (name == "left") {
				name_map["left"] = value;
			} else if (name == "back") {
				name_map["back"] = value;
			} else if (name == "side") {
				name_map["side"] = value;
			} else if (name == "top") {
				name_map["top"] = value;
			} else if (name == "bottom") {
				name_map["bottom"] = value;
			} else if (name == "inside") {
				name_map["inside"] = value;
			} else if (name == "border") {
				name_map["border"] = value;
			} else if (name == "vertical") {
				name_map["vertical"] = value;
			}
		}

		child = child.nextSibling();
	}

	return boost::shared_ptr<Operator>(new CompOperator(name_map));
}

boost::shared_ptr<Operator> parseCopyOperator(const QDomNode& node) {
	if (!node.toElement().hasAttribute("name")) {
		throw "copy node has to have name attribute.";
	}

	std::string copy_name = node.toElement().attribute("name").toUtf8().constData();

	return boost::shared_ptr<Operator>(new CopyOperator(copy_name));
}

boost::shared_ptr<Operator> parseExtrudeOperator(const QDomNode& node) {
	if (!node.toElement().hasAttribute("height")) {
		throw "extrude node has to have height attribute.";
	}

	std::string height = node.toElement().attribute("height").toUtf8().constData();

	return boost::shared_ptr<Operator>(new ExtrudeOperator(height));
}

boost::shared_ptr<Operator> parseSplitOperator(const QDomNode& node) {
	int splitAxis;
	std::vector<Value> sizes;
	std::vector<std::string> names;

	if (!node.toElement().hasAttribute("splitAxis")) {
		throw "split node has to have splitAxis attribute.";
	}
	if (node.toElement().attribute("splitAxis") == "x") {
		splitAxis = DIRECTION_X;
	} else if (node.toElement().attribute("splitAxis") == "y") {
		splitAxis = DIRECTION_Y;
	} else {
		splitAxis = DIRECTION_Z;
	}

	QDomNode child = node.firstChild();
	while (!child.isNull()) {
		if (child.toElement().tagName() == "param") {
			QString type = child.toElement().attribute("type");
			std::string value = child.toElement().attribute("value").toUtf8().constData();
			bool repeat = false;
			if (child.toElement().hasAttribute("repeat")) {
				repeat = true;
			}

			if (repeat) {
				if (type == "absolute") {
					sizes.push_back(Value(Value::TYPE_ABSOLUTE, value, true));
				} else if (type == "relative") {
					sizes.push_back(Value(Value::TYPE_RELATIVE, value, true));
				} else {
					sizes.push_back(Value(Value::TYPE_FLOATING, value, true));
				}
			} else {
				if (type == "absolute") {
					sizes.push_back(Value(Value::TYPE_ABSOLUTE, value));
				} else if (type == "relative") {
					sizes.push_back(Value(Value::TYPE_RELATIVE, value));
				} else {
					sizes.push_back(Value(Value::TYPE_FLOATING, value));
				}
			}

			names.push_back(child.toElement().attribute("name").toUtf8().constData());
		}

		child = child.nextSibling();
	}

	return boost::shared_ptr<Operator>(new SplitOperator(splitAxis, sizes, names));
}

}
