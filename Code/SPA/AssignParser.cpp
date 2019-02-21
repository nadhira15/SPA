#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "AssignParser.h"

using namespace std;

const int GLOBAL_CONST_ASSIGN_DELIMITER = '=';

bool parse(std::string statement) {
	string left = getLeft(statement);
	string right = getRight(statement);

	//Insert into PKB the details.
}

std::string getLeft(string assignStatement) {
	vector<string> tokenizedStmt = split(assignStatement, GLOBAL_CONST_ASSIGN_DELIMITER);
	return tokenizedStmt.at(0);
}

std::string getRight(string assignStatement) {
	vector<string> tokenizedStmt = split(assignStatement, GLOBAL_CONST_ASSIGN_DELIMITER);
	return tokenizedStmt.at(1);
}

std::vector<std::string> split(const std::string stmt, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(stmt);

	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

AssignParser::AssignParser()
{

}


AssignParser::~AssignParser()
{
}

std::string getLeft(std::string statement)
{

}

std::vector<std::string> split(std::string stmt, char delimiter)
{
	vector<string> v;

	string::size_type i = 0;
	//Find first occurence of delimiter.
	string::size_type j = stmt.find(delimiter);

	//Loop while there is still statement to parse.
	while (j != string::npos) {
		//Push result into vector.
		v.push_back(stmt.substr(i, j - i));
		//current j is ignore as it is the delimiter. We update i to after delimiter.
		i = j++;
		//find the new j.
		j = stmt.find(delimiter, j);

		//Push remaining into vector before exiting loop.
		if (j == string::npos)
			v.push_back(stmt.substr(i, stmt.length()));
	}

	return v;
}

std::string trim(std::string& str, std::string& whitespace)
{
	int strStart = str.find_first_not_of(whitespace);
	int strEnd = str.find_last_not_of(whitespace);

	if (strStart == std::string::npos) {
		return "";
	}

	int strLength = strEnd - strStart + 1;
	return str.substr(strStart, strLength);

}


