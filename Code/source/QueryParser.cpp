#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

#include "QueryParser.h"

const string whitespace = " \f\n\r\t\v";
const unordered_set<string> validVarType = { "stmt", "read", "print", "while", "if", "assign", "variable", "constant", "procedure" };

string QueryParser::parse(string query) {
	/*
	Main parser function for input query.
	Passes the following to query evaluator:
	...
	*/

	string errorString;

	// initial query validation
	errorString = initialValidation(query);
	if (errorString != "") {
		return errorString;
	}

	// splitting clauses
	vector<string> clauses = splitClauses(query);

	// validating clauses
	errorString = validateClauses(clauses);
	if (errorString != "") {
		return errorString;
	}

	// parsing declarations
	vector<pair<string, string>> declarations = splitDeclarations(clauses);

	// validating declarations
	errorString = validateDeclarations(declarations);
	if (errorString != "") {
		return errorString;
	}

	// parsing "Select" statement
	string selectStatement = clauses[clauses.size() - 1];

	vector<string> selectedVar;
	vector<pair<string, pair<string, string>>> suchThatCondition;
	vector<pair<string, pair<string, string>>> patternCondition;

	int suchThatIndex = selectStatement.find("such that");
	int patternIndex = selectStatement.find("pattern");
	int selectStatementLen = selectStatement.length();

	if (suchThatIndex == -1 && patternIndex == -1) {
		selectedVar = splitSelectParameter(selectStatement);
	}
	else if (suchThatIndex == -1 && patternIndex != -1) {
		selectedVar = splitSelectParameter(selectStatement.substr(0, suchThatIndex));
		suchThatCondition = splitSuchThatCondition(selectStatement.substr(suchThatIndex));
	}
	else if (suchThatIndex != -1 && patternIndex == 1) {
		selectedVar = splitSelectParameter(selectStatement.substr(0, patternIndex));
		patternCondition = splitPatternCondition(selectStatement.substr(patternIndex));
	}
	else if (suchThatIndex < patternIndex) {
		selectedVar = splitSelectParameter(selectStatement.substr(0, suchThatIndex));
		suchThatCondition = splitSuchThatCondition(selectStatement.substr(suchThatIndex, patternIndex - suchThatIndex));
		patternCondition = splitPatternCondition(selectStatement.substr(patternIndex));
	}
	else if (patternIndex < suchThatIndex) {
		selectedVar = splitSelectParameter(selectStatement.substr(0, patternIndex));
		patternCondition = splitPatternCondition(selectStatement.substr(patternIndex, suchThatIndex - patternIndex));
		suchThatCondition = splitSuchThatCondition(selectStatement.substr(patternIndex));
	}

	// validating 'Select' parameter
	errorString = validateSelectedVar(selectedVar, declarations);
	if (errorString != "") {
		return errorString;
	}

	// validating 'such that' parameter
	errorString = validateSuchThatParam(suchThatCondition);
	if (errorString != "") {
		return errorString;
	}

	// validating 'pattern' parameter
	errorString = validatePatternParam(patternCondition);
	if (errorString != "") {
		return errorString;
	}

	string result = evaluateSelectConditions(declarations, selectedVar, suchThatCondition, patternCondition);

	return result;
}

string QueryParser::initialValidation(string query) {
	/* 
	checks whether the input string is valid:
	- has length more than 0
	- has substring "Select"
	- has declaration
	output: error message (if any), otherwise empty string
	*/

	if (query.length() <= 0) {
		return "invalid query";
	}
	else if (query.find("Select") == -1) {
		return "invalid query";
	}
	else if (query.find("Select") == 0) {
		return "None";
	}
	else {
		return "";
	}
}

vector<string> QueryParser::splitClauses(string query) {
	/*
	Splitting the query string by char ";".
	Returns a vector<string> consisting of every declaration clauses (without trailing whitespaces) and the "Select" statement
	*/
	vector<string> output;
	char delimiter = ';';
	int startPoint = 0;
	int endPoint = query.find(delimiter);

	while (endPoint != -1) {
		output.push_back(removeTrailingWhitespaces(query.substr(startPoint, endPoint - startPoint)));
		startPoint = endPoint + 1;
		endPoint = query.find(delimiter, startPoint);
	}

	output.push_back(removeTrailingWhitespaces(query.substr(startPoint)));

	return output;
}

string QueryParser::validateClauses(vector<string> clauses) {
	/*
	Validate declaration clauses based on following conditions:
	- "Select" statement is the last statement
	- Each declaration clause consists of varType and varName
	*/

	int clausesSize = clauses.size();

	if (clauses[clausesSize - 1].find("Select") == -1) {
		return "invalid query";
	}

	for (int i = 0; i < clausesSize - 1; i++) {
		if (clauses[i].find(" ") == -1) {
			return "invalid query";
		}
	}

	return "";
}

vector<pair<string, string>> QueryParser::splitDeclarations(vector<string> clauses) {
	vector<pair<string, string>> output;
	int clausesSize = clauses.size();

	for (int i = 0; i < clausesSize - 1; i++) {
		string currentClauses = clauses[i];
		int charSpaceLoc = currentClauses.find_first_of(whitespace);
		string type = currentClauses.substr(0, charSpaceLoc);
		string varName = currentClauses.substr(charSpaceLoc);

		if (varName.find(",") != -1) {
			char delimiter = ',';
			int startPoint = 0;
			int endPoint = varName.find(delimiter);

			while (endPoint != -1) {
				string varNameSplitted = removeTrailingWhitespaces(varName.substr(startPoint, endPoint - startPoint));
				output.push_back(make_pair(type, varNameSplitted));
				startPoint = endPoint + 1;
				endPoint = varName.find(delimiter, startPoint);
			}

			output.push_back(make_pair(type, removeTrailingWhitespaces(varName.substr(startPoint))));
		}
		else {
			output.push_back(make_pair(type, removeTrailingWhitespaces(varName)));
		}
	}

	return output;
}

string QueryParser::validateDeclarations(vector<pair<string, string>> declarations) {
	// TODO: verify varName with lexical token

	for (size_t i = 0; i < declarations.size(); i++) {
		if (validVarType.find(declarations[i].first) == validVarType.end()) {
			return "invalid query";
		}
	}

	return "";
}

vector<string> QueryParser::splitSelectParameter(string selectStatement) {
	vector<string> output;

	int firstSpace = selectStatement.find_first_of(whitespace);
	string varName = removeAllWhitespaces(selectStatement.substr(firstSpace));

	output.push_back(varName);

	return output;
}

vector<pair<string, pair<string, string>>> QueryParser::splitSuchThatCondition(string suchThatClause) {
	vector<pair<string, pair<string, string>>> output;

	int openBracket = suchThatClause.find("(");
	int comma = suchThatClause.find(",");
	int closeBracket = suchThatClause.find(")");
	int strLen = suchThatClause.length();

	string condition = removeAllWhitespaces(suchThatClause.substr(9, openBracket - 9));
	string firstVar = removeAllWhitespaces(suchThatClause.substr(openBracket + 1, comma - openBracket - 1));
	string secondVar = removeAllWhitespaces(suchThatClause.substr(comma + 1, closeBracket - comma - 1));

	output.push_back(make_pair(condition, make_pair(firstVar, secondVar)));

	return output;
}

vector<pair<string, pair<string, string>>> QueryParser::splitPatternCondition(string patternClause) {
	vector<pair<string, pair<string, string>>> output;

	int openBracket = patternClause.find("(");
	int comma = patternClause.find(",");
	int closeBracket = patternClause.find(")");
	int strLen = patternClause.length();

	string varName = removeAllWhitespaces(patternClause.substr(7, openBracket - 7));
	string firstPattern = removeAllWhitespaces(patternClause.substr(openBracket + 1, comma - openBracket - 1));
	string secondPattern = removeAllWhitespaces(patternClause.substr(comma + 1, closeBracket - comma - 1));

	output.push_back(make_pair(varName, make_pair(firstPattern, secondPattern)));

	return output;
}

string QueryParser::validateSelectedVar(vector<string> selectedVar, vector<pair<string, string>> declarations) {

	return "";
}

string QueryParser::validateSuchThatParam(vector<pair<string, pair<string, string>>> param) {

	return "";
}

string QueryParser::validatePatternParam(vector<pair<string, pair<string, string>>> param) {

	return "";
}

string QueryParser::evaluateSelectConditions(vector<pair<string, string>> declarations, 
	vector<string> selectedVar, vector<pair<string, pair<string, string>>> suchThatCondition,
	vector<pair<string, pair<string, string>>> patternCondition) {
	
	// TODO: evaluate the result of select conditions

	return "";
}

string QueryParser::removeAllWhitespaces(string s) {
	s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
	return s;
}

string QueryParser::removeTrailingWhitespaces(string s) {
	string trimRight = s.substr(0, s.find_last_not_of(whitespace) + 1);
	string trimLeft = trimRight.substr(trimRight.find_first_not_of(whitespace));
	return trimLeft;
}
