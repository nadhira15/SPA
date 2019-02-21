#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>

class AssignParser
{
public:
	std::string parseLeft(short stmtNo, std::string statement);
	std::string parseRight(short stmtNo, std::string statement);
	AssignParser();
	~AssignParser();
};
