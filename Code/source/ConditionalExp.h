#pragma once
#include<string>
#include<vector>


namespace ConditionalExp {

	bool verifyConditionalExp(const std::string statement);
	std::vector<std::string> getVariables();
	std::vector<std::string> getConstants();
};