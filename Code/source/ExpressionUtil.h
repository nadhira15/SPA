#pragma once

#include<stdio.h>
#include <vector>

using namespace std;

namespace ExpressionUtil
{
	string convertInfixToPrefix(string expression);

	void expressionPreprocess(std::string &expression);

	vector<string> getVariables(string infixExpression);

	vector<string> getConstants(string infixExpression);

	bool verifyInfixExpression(string infixExpression);
	bool checkParenthesis(string infixExpression);
	int getPriority(string p);
	bool isOperator(char c);
}