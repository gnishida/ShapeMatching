#pragma once

#include <map>
#include "Rule.h"
#include <QFile>
#include <QDomDocument>
#include <QDomNode>

namespace cga {

void parseRules(const std::string& filename, RuleSet& ruleSet);
//void parseRule(const std::string& filename, std::vector<Rule>& rules);
void parseRuleNode(const QDomElement& rule_node, Rule& rule);
boost::shared_ptr<Operator> parseCompOperator(const QDomNode& node);
boost::shared_ptr<Operator> parseCopyOperator(const QDomNode& node);
boost::shared_ptr<Operator> parseExtrudeOperator(const QDomNode& node);
boost::shared_ptr<Operator> parseSplitOperator(const QDomNode& node);

}