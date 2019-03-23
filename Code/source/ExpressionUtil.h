#pragma once

#include <string>
#include <regex>
#include <stack>
#include <vector>
#include "LexicalToken.h"
#include "StringUtil.h"

/*
 * This namespace provides a suite of functions that handles expressions
 * in the SPA project. It allows for :
 * (1) Verification of the correctness of the expression
 *     - verifyInfixExpression() takes in the expression as an argument and returns
 *       true if valid it also checks that all constants and variables inside are valid.
 * (2) Conversion of expression from Infix Expression to Prefix Expression
 *     - convertInfixToPrefix() converts an infix expression to a prefix expression.
 * (3) Extraction of Constants and Variables of a given Infix Expression.
 *     - getVariables() extracts all valid variables from the Infix expression
 *     - getConstants() extracts all valid constants from the Infix expression
 * 
 * In order to use feature (2) and (3). It is important to perform a verification
 * of the correctness of the expression by using feature (1) beforehand.
 */
namespace ExpressionUtil
{
	// Functions that are meant pertain to Expression Checking.

	// Converts an Infix Expression to a Prefix Expression.
	std::string convertInfixToPrefix(std::string expression);

	// Gets the list of variables of an Infix Expression
	std::vector<std::string> getVariables(std::string infixExpression);

	// Gets the list of constants found in an Infix Expression
	std::vector<std::string> getConstants(std::string infixExpression);

	// A boolean check to see if Expression is a valid Infix Expression
	bool verifyInfixExpression(std::string infixExpression);

	// Additional functions used by the namespace.
	void expressionPreprocess(std::string &expression);

	bool checkParenthesis(std::string infixExpression);

	int getPriority(std::string p);

	bool isOperator(char c);
}