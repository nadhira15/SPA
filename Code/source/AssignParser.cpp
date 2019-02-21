#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>
#include "AssignParser.h"

using namespace std;

std::regex assignStmt("^[\s]*([\w]+)[\s]*[=][\s]*(.+)[\s]*$");

bool AssignParser::parse(short stmtNo, std::string statement) {
	
	if (std::regex_match(statement, assignStmt)) {
		smatch result;
		regex_search(statement, result, assignStmt);
		string left = result[0];
		string right = result[1];

		return true;
	}
	else {
		return false;
	}
}

AssignParser::AssignParser()
{

}


AssignParser::~AssignParser()
{
}