#include "Rule.h"
#include "CGA.h"
#include "Shape.h"
#include "NumberEval.h"
#include <sstream>
#include <boost/algorithm/string/replace.hpp>

namespace cga {

float Value::getEstimateValue(float size, const RuleSet& ruleSet, const boost::shared_ptr<Shape>& shape) const {
	if (type == Value::TYPE_ABSOLUTE) {
		return ruleSet.evalFloat(value, shape);
	} else if (type == Value::TYPE_RELATIVE) {
		return ruleSet.evalFloat(value, shape) * size;
	} else {
		return ruleSet.evalFloat(value, shape);
	}
}

/**
 * このルールを指定されたshapeに適用する。
 * いくつかのオペレーション (compやsplitなど)は、適用後のshapeをstackに格納する。
 *
 * @param shape		shape
 * @param ruleSet	全ルール
 * @param stack		stack
 */
void Rule::apply(boost::shared_ptr<Shape>& shape, const RuleSet& ruleSet, std::list<boost::shared_ptr<Shape> >& stack) const {
	for (int i = 0; i < operators.size(); ++i) {
		shape = operators[i]->apply(shape, ruleSet, stack);
		if (shape == NULL) break;
	}
	
	if (shape != NULL) {
		if (operators.size() == 0 || operators.back()->name == "copy") {
			// copyで終わる場合、このshapeはもう必要ないので削除
			//delete shape;
			//shape = NULL;
			shape = boost::shared_ptr<Shape>();
		} else {
			// copyで終わらない場合、このshapeは描画する必要があるので、残す。
			// 同じ名前でstackに格納すると無限再帰してしまうため、末尾に!を付加した名前にして格納する。
			std::stringstream ss;
			ss << shape->_name << "!";
			shape->_name = ss.str();
			stack.push_back(shape);
		}
	}
}

/**
 * 指定されたsizeをsplitした後の、各断片のサイズを計算する。
 *
 * @param size							もとのsize
 * @param sizes							指定された、各断片のサイズ
 * @param output_names					指定された、各断片の名前
 * @param ruleSet						ルール (sizeなどで変数が使用されている場合、解決するため)
 * @param decoded_sizes	[OUT]			計算された、各断片のサイズ
 * @param decoded_output_names [OUT]	計算された、各断片の名前
 */
void Rule::decodeSplitSizes(float size, const std::vector<Value>& sizes, const std::vector<std::string>& output_names, const RuleSet& ruleSet, const boost::shared_ptr<Shape>& shape, std::vector<float>& decoded_sizes, std::vector<std::string>& decoded_output_names) {
	float regular_sum = 0.0f;
	float floating_sum = 0.0f;
	int repeat_count = 0;

	for (int i = 0; i < sizes.size(); ++i) {
		if (sizes[i].repeat) {
			repeat_count++;
		} else {
			if (sizes[i].type == Value::TYPE_ABSOLUTE) {
				regular_sum += ruleSet.evalFloat(sizes[i].value, shape);
			} else if (sizes[i].type == Value::TYPE_RELATIVE) {
				regular_sum += size * ruleSet.evalFloat(sizes[i].value, shape) * size;
			} else if (sizes[i].type == Value::TYPE_FLOATING) {
				floating_sum += ruleSet.evalFloat(sizes[i].value, shape);
			}
		}
	}

	float floating_scale = 1.0f;
	if (floating_sum > 0 && repeat_count == 0) {
		floating_scale = (size - regular_sum) / floating_sum;
	}

	for (int i = 0; i < sizes.size(); ++i) {
		if (sizes[i].repeat) {
			float s = sizes[i].getEstimateValue(size - regular_sum - floating_sum * floating_scale, ruleSet, shape);
			int num = (size - regular_sum - floating_sum * floating_scale) / s;
			s = (size - regular_sum - floating_sum * floating_scale) / num;
			for (int k = 0; k < num; ++k) {
				decoded_sizes.push_back(s);
				decoded_output_names.push_back(output_names[i]);
			}
		} else {
			if (sizes[i].type == Value::TYPE_ABSOLUTE) {
				decoded_sizes.push_back(ruleSet.evalFloat(sizes[i].value, shape));
				decoded_output_names.push_back(output_names[i]);
			} else if (sizes[i].type == Value::TYPE_RELATIVE) {
				decoded_sizes.push_back(ruleSet.evalFloat(sizes[i].value, shape) * size);
				decoded_output_names.push_back(output_names[i]);
			} else if (sizes[i].type == Value::TYPE_FLOATING) {
				decoded_sizes.push_back(ruleSet.evalFloat(sizes[i].value, shape) * floating_scale);
				decoded_output_names.push_back(output_names[i]);
			}
		}
	}
}

void RuleSet::clear() {
	attrs.clear();
	rules.clear();
}

bool RuleSet::contain(const std::string& name) const {
	if (rules.find(name) == rules.end()) return false;
	else return true;
}

/**
 * 変数とその値を追加する。
 *
 * @param name		変数名
 * @param value		値
 */
void RuleSet::addAttr(const std::string& name, const std::string& value) {
	attrs[name] = value;
}

/**
 * ルールを追加する。
 *
 * @param name		ルール名 (左辺に来るnonterminalの名前)
 */
void RuleSet::addRule(const std::string& name) {
	rules[name].operators.clear();
}

/**
 * ルールにオペレーションを追加する。
 *
 * @param name		ルール名
 * @param op		オペレーション
 */
void RuleSet::addOperator(const std::string& name, const boost::shared_ptr<Operator>& op) {
	rules[name].operators.push_back(op);
}

/**
 * 指定された変数を、数値に変換する。
 *
 * @param attr_name		変数名
 * @param shape			shape
 * @return				変換された数値
 */
float RuleSet::evalFloat(const std::string& attr_name, const boost::shared_ptr<Shape>& shape) const {
	myeval::calculator<std::string::const_iterator> calc;

	myeval::variables.clear();
	myeval::variables.add("scope.sx", shape->_scope.x);
	myeval::variables.add("scope.sy", shape->_scope.y);
	myeval::variables.add("scope.sz", shape->_scope.z);

	for (auto it = attrs.begin(); it != attrs.end(); ++it) {
		float val;
		if (sscanf(it->second.c_str(), "%f", &val) != EOF) {
			myeval::variables.add(it->first, val);
		}
	}

	float result;
	std::string::const_iterator iter = attr_name.begin();
	std::string::const_iterator end = attr_name.end();
	bool r = phrase_parse(iter, end, calc, boost::spirit::ascii::space, result);
	if (r && iter == end) {
		return result;
	} else {
		std::string rest(iter, end);
		std::cout << "Parsing failed\n";
		std::cout << "stopped at: \": " << rest << "\"\n";
		throw "Parsing failed\nstpped at: \": " + rest + "\"\n";
	}
}

/**
 * 指定された変数を、文字列に変換する。
 *
 * @param attr_name		変数名
 * @param shape			shape
 * @return				変換された文字列
 */
std::string RuleSet::evalString(const std::string& attr_name, const boost::shared_ptr<Shape>& shape) const {
	if (attrs.find(attr_name) == attrs.end()) {
		return attr_name;
	} else {
		return attrs.at(attr_name);
	}
}

void RuleSet::merge(const RuleSet& ruleSet) {
	for (auto it = ruleSet.attrs.begin(); it != ruleSet.attrs.end(); ++it) {
		this->attrs[it->first] = it->second;
	}
	for (auto it = ruleSet.rules.begin(); it != ruleSet.rules.end(); ++it) {
		this->rules[it->first] = it->second;
	}
}

}
