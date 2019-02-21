#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>
#include "AssignParser.h"

using namespace std;

std::regex assignStmt("^[\\s]*([\\w]+)[\\s]*[=][\\s]*(.+)[\\s]*$");

std::string AssignParser::parseLeft(short stmtNo, std::string statement) {
	
	if (std::regex_match(statement, assignStmt)) {
		smatch result;
		regex_search(statement, result, assignStmt);
		string left = result[1];

		return left;
	}
	else {
		throw "failed";
	}
}

std::string AssignParser::parseRight(short stmtNo, std::string statement) {

	if (std::regex_match(statement, assignStmt)) {
		smatch result;
		regex_search(statement, result, assignStmt);
		string right = result[2];

		return right;
	}
	else {
		throw "failed";
	}
}

AssignParser::AssignParser()
{

}


AssignParser::~AssignParser()
{
}