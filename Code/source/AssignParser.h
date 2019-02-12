#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class AssignParser
{
public:
	AssignParser();
	~AssignParser();

	bool parse(std::string statement);

	std::string getLeft(std::string assignStatement);

	std::string getRight(std::string assignStatement);

	std::vector<std::string> split1(const std::string stmt, char delimiter);

	std::vector<std::string> split(const std::string stmt, char delimiter);


	std::string trim(std::string & str, std::string & whitespace);
};

#pragma once

