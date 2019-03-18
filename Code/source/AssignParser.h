#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>

#include "ExpressionUtil.h"
#include "LexicalToken.h"

class AssignParser
{
public:
	std::string getLeft(std::string statement);
	std::vector<std::string> getRightVariable(std::string statement);
	std::vector<std::string> getRightConstant(std::string statement);
	std::string getPrefixExpression(std::string statement);
	AssignParser();
	~AssignParser();

private:
	std::string parseLeft(std::string statement);
	std::string parseRight(std::string statement);
};
