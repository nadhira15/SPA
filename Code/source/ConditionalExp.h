#pragma once
#include<string>
#include<vector>

class ConditionalExp {
public:
	static std::vector<std::string> getVariables();
	static std::vector<std::string> getConstants();
	static bool varifyConditionalExp(const std::string expression);
};