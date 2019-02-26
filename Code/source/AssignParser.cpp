#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>
#include "AssignParser.h"
#include "LexicalToken.h"
#include "ExpressionUtil.h"

using namespace std;
using namespace LexicalToken;
using namespace ExpressionUtil;

std::regex assignStmt("^[\\s]*([\\w]+)[\\s]*[=][\\s]*(.+)[\\s]*$");
vector<string> expressionVariables;
vector<string> expressionConstants;
string prefixExpression;

std::string AssignParser::parseLeft(std::string statement) {
	
	if (std::regex_match(statement, assignStmt)) {
		smatch result;
		regex_search(statement, result, assignStmt);
		string left = result[1];

		return left;
	}
	else {
		throw "Invalid Assign Expression!";
	}
}

std::string AssignParser::parseRight(std::string statement) {

	if (std::regex_match(statement, assignStmt)) {
		smatch result;
		regex_search(statement, result, assignStmt);
		string right = result[2];

		return right;
	}
	else {
		throw "Invalid Assign Expression!";
	}
}

string AssignParser::getLeft(string statement) {
	string variable = parseLeft(statement);
	bool isValidName = verifyName(variable);
	if (isValidName) {
		return variable;
	}
	else {
		throw "Invalid Variable Name!";
	}
}

vector<string> AssignParser::getRightVariable(string statement) {
	string expression = parseRight(statement);
	bool isValidExpression = verifyInfixExpression(expression);

	vector<string> listVariable;
	
	if (isValidExpression) {
		listVariable = ExpressionUtil::getVariables(expression);
	}
	else {
		throw "Invalid Expression in Assign Statement";
	}

	return listVariable;

}

vector<string> AssignParser::getRightConstant(string statement) {
	string expression = parseRight(statement);
	bool isValidExpression = verifyInfixExpression(expression);

	vector<string> listConstant;

	if (isValidExpression) {
		listConstant = ExpressionUtil::getConstants(expression);
	}
	else {
		throw "Invalid Expression in Assign Statement";
	}

	return listConstant;

}

string AssignParser::getPostfixExpression(string statement) {
	string expression = parseRight(statement);
	bool isValidExpression = verifyInfixExpression(expression);

	string postfixExpression;

	if (isValidExpression) {
		postfixExpression = ExpressionUtil::convertInfixToPrefix(expression);
	}
	else {
		throw "Invalid Expression in Assign Statement";
	}

	return postfixExpression;
}

AssignParser::AssignParser()
{

}


AssignParser::~AssignParser()
{
}