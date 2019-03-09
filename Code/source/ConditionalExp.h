#pragma once
#include<string>
#include<vector>

class ConditionalExp {
public:
	std::vector<std::string> getVariables();
	std::vector<std::string> getConstants();
	bool verifyConditionalExp(const std::string statement);
private:
	void populateVector();
	std::vector<std::string>wordVector;
	std::vector<std::string>variableVector;
	std::vector<std::string>variables;
	std::vector<std::string>constants;
	std::string str;
};