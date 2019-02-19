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
	AssignParser();
	~AssignParser();

	bool parse(short stmtNo, std::string statement);
};

#pragma once

