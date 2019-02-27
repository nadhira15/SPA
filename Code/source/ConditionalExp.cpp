#include <stdio.h>
#include <string>
#include <regex>
#include"ConditionalExp.h"

using namespace std;

std::vector<string> expressions;
std::vector<string> variables;
std::vector<string> constants;

bool ConditionalExp::verifyConditionalExp(std::string statement) {
	std::regex format("\\((.+)\\)(?:\\&\\&|\\|\\|)\\((.+)\\)");
	std::smatch expressionMatch;
	if (std::regex_search(statement, expressionMatch, format)){
		std::string left = expressionMatch.str(1);
		std::string right = expressionMatch.str(2);
		if (populateVector(left) && populateVector(right)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (populateVector(statement)) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool ConditionalExp::populateVector(std::string expression) {
	std::regex variableRegex("[a-zA-Z]+\\d*");
	std::regex constantRegex("((?![a-zA-Z])\\d+(?![a-zA-Z]))");
	std::regex formular("(!?[\\(\\s]*(?:[a-zA-Z]+\\d*)|(?:\\d+))\\s*([\\+\\-\\*/%<>]|[<>!=]=)[\\(\\s]*((?:[a-zA-Z]+\\d*)|(?:\\d+))[\\(\\s\\)]*(([\\(\\s\\)]*([\\+\\-\\*/%<>]|[<>!=]=)[\\(\\s\\)]*(([a-zA-Z]+\\d*)|(\\d+)))*[\\(\\s\\)]*)");
	//verify brackets
	int count = 0;
	for (std::string::iterator it = expression.begin(); it != expression.end(); ++it) {
		if (*it == '(') {
			count++;
		}
		else if (*it == ')') {
			count--;
		}
		if (count < 0)
			return false;
	}
	if (std::regex_match(expression, formular)) {
		std::string toGetVariables = expression;
		std::string toGetConstants = expression;
		std::smatch variableMatch;
		std::smatch constantMatch;
		while (std::regex_search(toGetVariables, variableMatch, variableRegex)) {
			variables.push_back(variableMatch.str(0));
			toGetVariables = variableMatch.suffix().str();
		}
		while (std::regex_search(toGetConstants, constantMatch, constantRegex)) {
			constants.push_back(constantMatch.str(0));
			toGetConstants = constantMatch.suffix().str();
		}
		return true;
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