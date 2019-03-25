#pragma once
#include "AssignParser.h"

std::regex assignStmt("^[\\s]*([\\w]+)[\\s]*[=][\\s]*(.+)[\\s]*$");
std::vector<std::string> expressionVariables;
std::vector<std::string> expressionConstants;
std::string prefixExpression;

std::string AssignParser::parseLeft(std::string statement) {
	
	if (std::regex_match(statement, assignStmt)) {
		std::smatch result;
		std::regex_search(statement, result, assignStmt);
		std::string left = result[1];

		return left;
	}
	else {
		throw "Invalid Assign Statement! Statement : " + statement;
	}
}

std::string AssignParser::parseRight(std::string statement) {

	if (std::regex_match(statement, assignStmt)) {
		std::smatch result;
		std::regex_search(statement, result, assignStmt);
		std::string right = result[2];

		return right;
	}
	else {
		throw "Invalid Assign Statement! Statement : " + statement;
	}
}

std::string AssignParser::getLeft(std::string statement) {
	std::string variable = parseLeft(statement);
	bool isValidName = LexicalToken::verifyName(variable);
	if (isValidName) {
		return variable;
	}
	else {
		throw "Invalid Variable Name! Variable : " + variable;
	}
}

std::vector<std::string> AssignParser::getRightVariable(std::string statement) {
	std::string expression = parseRight(statement);
	bool isValidExpression = ExpressionUtil::verifyInfixExpression(expression);

	std::vector<std::string> listVariable;
	
	if (isValidExpression) {
		listVariable = ExpressionUtil::getVariables(expression);
	}
	else {
		throw "Invalid Expression in Assign Statement! Expression : " + expression; 
	}

	return listVariable;

}

std::vector<std::string> AssignParser::getRightConstant(std::string statement) {
	std::string expression = parseRight(statement);
	bool isValidExpression = ExpressionUtil::verifyInfixExpression(expression);

	std::vector<std::string> listConstant;

	if (isValidExpression) {
		listConstant = ExpressionUtil::getConstants(expression);
	}
	else {
		throw "Invalid Expression in Assign Statement!Expression : " + expression;
	}

	return listConstant;

}

std::string AssignParser::getPrefixExpression(std::string statement) {
	std::string expression = parseRight(statement);
	bool isValidExpression = ExpressionUtil::verifyInfixExpression(expression);

	std::string postfixExpression;

	if (isValidExpression) {
		postfixExpression = ExpressionUtil::convertInfixToPrefix(expression);
	}
	else {
		throw "Invalid Expression in Assign Statement!Expression : " + expression;
	}

	return postfixExpression;
}

AssignParser::AssignParser()
{

}


AssignParser::~AssignParser()
{
}