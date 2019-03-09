#pragma once

#include<stdio.h>
#include <vector>

using namespace std;

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
	string convertInfixToPrefix(string expression);

	// Gets the list of variables of an Infix Expression
	vector<string> getVariables(string infixExpression);

	// Gets the list of constants found in an Infix Expression
	vector<string> getConstants(string infixExpression);

	// A boolean check to see if Expression is a valid Infix Expression
	bool verifyInfixExpression(string infixExpression);

	// Additional functions used by the namespace.
	void expressionPreprocess(std::string &expression);

	bool checkParenthesis(string infixExpression);

	int getPriority(string p);

	bool isOperator(char c);
}