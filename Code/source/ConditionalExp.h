#pragma once
#include<string>
#include<vector>

class ConditionalExp {
public:
	static std::vector<std::string> getVariables();
	static std::vector<std::string> getConstants();
	static bool verifyConditionalExp(const std::string statement);
private:
	static void populateVector();
};