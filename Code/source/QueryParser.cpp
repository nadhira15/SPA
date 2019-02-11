#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "QueryParser.h"

string QueryParser::parse(string query) {
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
	vector<pair<string, string>> selectConditions = splitSelectConditions(selectStatement);

	// validating "Select" conditions
	errorString = validateSelectConditions(selectConditions);
	if (errorString != "") {
		return errorString;
	}

	string result = evaluateSelectConditions(declarations, selectConditions);
	
	return result;
}

string QueryParser::initialValidation(string query) {
	if (query.length() <= 0) {
		return "invalid query";
	}
	else if (query.find("Select") == -1) {
		// query string has no substring "Select"
		return "invalid query";
	}
	else if (query.find("Select") == 0) {
		// missing declaration
		return "None";
	}
	else {
		return "";
	}
}

vector<string> QueryParser::splitClauses(string query) {
	vector<string> output;
	char delimiter = ';';
	int startPoint = 0;
	int endPoint = query.find(delimiter);
	int size = 0;

	while (endPoint != -1) {
		output.push_back(query.substr(startPoint, endPoint - startPoint));
		startPoint = endPoint + 1;
		endPoint = query.find(delimiter, startPoint);
		size = size + 1;
	}

	output.push_back(query.substr(startPoint));

	return output;
}

string QueryParser::validateClauses(vector<string> clauses) {
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
	char delimiter = ' ';

	for (int i = 0; i < clausesSize - 1; i++) {
		string currentClauses = clauses[i];
		int charSpaceLoc = currentClauses.find(delimiter);
		string type = currentClauses.substr(0, currentClauses.length() - charSpaceLoc);
		string varName = currentClauses.substr(charSpaceLoc);
		output.push_back(make_pair(type, varName));
	}

	return output;
}

string QueryParser::validateDeclarations(vector<pair<string, string>> declarations) {
	// TODO: check whether each declaration has valid type and valid varName
	// need Lexical Token Verification for varName
	return "";
}

vector<pair<string, string>> QueryParser::splitSelectConditions(string selectStatement) {
	vector<pair<string, string>> output;

	return output;
}

string QueryParser::validateSelectConditions(vector<pair<string, string>> selectConditions) {
	// TODO: check whether each declaration has valid type and valid varName

	return "";
}

string QueryParser::evaluateSelectConditions(vector<pair<string, string>> declarations, vector<pair<string, string>> selectConditions) {
	// TODO: evaluate the result of select conditions

	return "";
}