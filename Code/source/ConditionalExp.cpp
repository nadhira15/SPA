#include <stdio.h>
#include <string>
#include <regex>
#include"ConditionalExp.h"

using namespace std;

std::vector<string> variables;
std::vector<string> constants;

bool ConditionalExp::varifyConditionalExp(std::string expression) {
	std::regex variableRegex("[a-zA-Z]+\\d*");
	std::regex constantRegex("((?![a-zA-Z])\\d+(?![a-zA-Z])");
	std::regex formular("((?:[a-zA-Z]+\\d*)|(?:\\d+))\\s*([\\+\\-\\*/%] | [<> != ] = )\\s*((? : [a - zA - Z] + \\d*) | (? : \\d + ))\\s*((\\s*([\\ + \\ - \\* / %] | [<> != ] = )\\s*(([a - zA - Z] + \\d*) | (\\d + )))*)");
	if (std::regex_match(expression,formular)) {
		std::string toGetVariables = expression;
		std::string toGetConstants = expression;
		std::smatch variableMatch;
		std::smatch constantMatch;
		while (std::regex_search(toGetVariables, variableMatch, variableRegex)) {
			variables.push_back(variableMatch.str(1));
			toGetVariables = variableMatch.suffix().str();
		}
		while (std::regex_search(toGetConstants, constantMatch, constantRegex)) {
			constants.push_back(constantMatch.str(1));
			toGetConstants = constantMatch.suffix().str();
		}
	}
	else {
		return false;
	}
}

std::vector<std::string> ConditionalExp::getVariables() {
	return variables;
}

std::vector<std::string> ConditionalExp::getConstants() {
	return constants;
}