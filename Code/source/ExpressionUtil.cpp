#include <string>
#include <regex>
#include <stack>
#include "ExpressionUtil.h"
#include "LexicalToken.h"
#include "StringUtil.h"

using namespace std;
using namespace ExpressionUtil;
using namespace LexicalToken;
using namespace StringUtil;

regex terms("(\\w+)");
regex validExpression("^(?:\(?\\s*(\\w+)\\s*\)?)(?:\\s*[+\*\-%\/]\\s*(?:\(?\\s*(\\w+)\\s*\)?))*$");

//Convert Infix Expressions into Prefix expression for easier string matching
string ExpressionUtil::convertInfixToPrefix(string expression) {
	expressionPreprocess(expression);

	stack<string> operatorStack;
	stack<string> operandStack;

	//Split by spaces to create substrings.
	char spaceDelimiter = ' ';
	vector<string> expressionVector = split(expression, spaceDelimiter);

	for (int i = 0; i < expressionVector.size(); i++) {
		//Push opening bracket to operator stack
		if (expressionVector[i] == "(") {
			operatorStack.push(expressionVector[i]);
		}

		//If Closing bracket, pop until we get back opening bracket.
		else if (expressionVector[i] == ")") {

			while (!operatorStack.empty() && operatorStack.top() != "(") {

				string operand1 = operandStack.top();
				operandStack.pop();

				string operand2 = operandStack.top();
				operandStack.pop();

				string operator1 = operatorStack.top();
				operatorStack.pop();

				//form sub-expression

				string subexpre = operator1 + " " + operand1 + " " + operand2;
				operandStack.push(subexpre);
			}

			//Opening bracket popped.
			operatorStack.pop();
		}

		//If operand push operand Stack
		else if (!isOperator(expressionVector[i].c_str[0])) {
			operandStack.push(expressionVector[i]);
		}

		//If operator push to operator stack only after performing higher priority operator.
		else {
			while (!operatorStack.empty() && getPriority(expressionVector[i]) <= getPriority(operatorStack.top())) {
				string operand1 = operandStack.top();
				operandStack.pop();

				string operand2 = operandStack.top();
				operandStack.pop();

				string operator1 = operatorStack.top();
				operatorStack.pop();

				//form sub-expression

				string subexpre = operator1 + " " + operand1 + " " + operand2;
				operandStack.push(subexpre);
			}
			operatorStack.push(expressionVector[i]);
		}
	}

	while (!operatorStack.empty) {
		string operand1 = operandStack.top();
		operandStack.pop();

		string operand2 = operandStack.top();
		operandStack.pop();

		string operator1 = operatorStack.top();
		operatorStack.pop();

		//form sub-expression

		string subexpre = operator1 + " " + operand1 + " " + operand2;
		operandStack.push(subexpre);
	}
			
	return operandStack.top();

}

void ExpressionUtil::expressionPreprocess(std::string &expression)
{
	//Remove every single whitespace
	expression.erase(remove_if(expression.begin(), expression.end(), isspace), expression.end());

	vector<string> operators{ "+", "-", "*", "/", "%" , "(", ")" };

	//Add a space at every occurence of an operator.
	for (string op : operators) {
		int pos = 0;
		while (true) {
			pos = expression.find(op, pos);

			//Finishing condition
			if (pos == string::npos) {
				break;
			}

			//Don't add if at the start.
			if (pos != 0) {
				expression.insert(pos, " ");
				pos++;
			}

			//Account for the actual operator
			pos++;

			//Don't add if at the end.
			if (pos >= expression.size()) {
				break;
			}


			expression.insert(pos, " ");
			pos++;
		}
	}
}

vector<string> ExpressionUtil::getVariables(string infixExpression) {
	smatch result;
	regex_search(infixExpression, result, terms);

	vector<string> variableVector;

	for (int i = 0; i < result.size(); i++) {
		bool isValidName = verifyName(result[i]);
		if (isValidName) {
			variableVector.push_back(result[i]);
		}
	}

	return variableVector;
}

vector<string> ExpressionUtil::getConstants(string infixExpression) {
	smatch result;
	regex_search(infixExpression, result, terms);

	vector<string> constantVector;

	for (int i = 0; i < result.size(); i++) {
		bool isValidInteger = verifyInteger(result[i]);
		if (isValidInteger) {
			constantVector.push_back(result[i]);
		}
	}

	return constantVector;
}

//Check if expression given is a valid Infix expression. Perform this step before running convert to Infix.
bool ExpressionUtil::verifyInfixExpression(string infixExpression) {
	if (regex_match(infixExpression, validExpression)) {
		if (checkParenthesis(infixExpression)) {
			smatch result;
			regex_search(infixExpression, result, terms);

			for (int i = 0; i < result.size(); i++) {
				bool isValidName = verifyName(result[i]);
				bool isValidInteger = verifyInteger(result[i]);
				if (!isValidName && !isValidInteger) {
					return false;
				}
			}
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

//Checks if there is matching parenthesis.
bool ExpressionUtil::checkParenthesis(string infixExpression) {
	stack<char> bracketStack;

	for (int i = 0; i < infixExpression.length; i++) {
		if (infixExpression.at(i) == '(') {
			bracketStack.push('(');
		}
		else if (infixExpression.at(i) == ')') {
			char stackTop = bracketStack.top();
			if (stackTop == NULL) {
				return false;
			}
			else {
				bracketStack.pop();
			}
		}
	}

	if (bracketStack.empty) {
		return true;
	}
	else {
		return false;
	}
}

// Get priority of the operator. If is not a operator return 0.
int getPriority(string p)
{
	char c = p.c_str()[0];
	if (c == '-' || c == '+')
		return 1;
	else if (c == '*' || c == '/' || c == '%')
		return 2;
	return 0;
}

bool isOperator(char c)
{
	return (!isalpha(c) && !isdigit(c));
}


