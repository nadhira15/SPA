#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <limits>

using namespace std;

#include "QueryEvaluator.h"
#include "QueryParser.h"
#include "QueryValidator.h"
#include "StringUtil.h"

string whitespace = " \f\n\r\t\v";
int maxInt = numeric_limits<int>::max();

/*
Main parser function for input query.
Passes the following to query evaluator:
1. vector<pair<string, string>> declarations
2. vector<string> selectedVar
3. vector<pair<string, pair<string, string>>> suchThatCondition
4. vector<pair<string, pair<string, string>>> patternCondition
*/
unordered_set<string> QueryParser::parse(string query) {
	
	string errorString;
	unordered_set<string> result;

	// initial query validation
	errorString = QueryValidator::initialValidation(query);
	if (errorString != "") {
		result.insert("error");
		return result;
	}

	// splitting clauses
	vector<string> clauses = splitClauses(query);

	// validating clauses
	errorString = QueryValidator::validateClauses(clauses);
	if (errorString != "") {
		result.insert("error");
		return result;
	}

	// parsing declarations
	vector<pair<string, string>> declarations = splitDeclarations(clauses);

	unordered_map<string, string> declarationsMap;
	for (int i = 0; i < declarations.size(); i++) {
		declarationsMap.insert(make_pair(declarations[i].second, declarations[i].first));
	}

	// validating declarations
	errorString = QueryValidator::validateDeclarations(declarations);
	if (errorString != "") {
		result.insert("error");
		return result;
	}

	// parsing "Select" statement
	string selectStatement = clauses[clauses.size() - 1];

	vector<string> selectedVar;
	vector<pair<string, pair<string, string>>> suchThatCondition;
	vector<pair<string, pair<string, string>>> patternCondition;

	vector<string> suchThatClauses;
	vector<string> patternClauses;
	vector<string> withClauses;
	int suchThatIndex = selectStatement.find("such that");
	int patternIndex = selectStatement.find("pattern");
	int withIndex = selectStatement.find("with");
	int andIndex = maxInt;
	string previousClause;

	vector<int> indexes = { suchThatIndex, patternIndex, withIndex };
	int nextIndex = getMinimumValue(indexes);

	if (nextIndex == -1) {
		selectedVar = splitSelectParameter(selectStatement);
	}
	else {
		selectedVar = splitSelectParameter(selectStatement.substr(0, nextIndex));
		selectStatement = selectStatement.substr(nextIndex);
	}

	while (selectStatement.length() > 0 && nextIndex != -1) {
		suchThatIndex = selectStatement.substr(1).find("such that");
		patternIndex = selectStatement.substr(1).find("pattern");
		withIndex = selectStatement.substr(1).find("with");
		andIndex = selectStatement.substr(1).find("and");
		indexes = { suchThatIndex, patternIndex, withIndex, andIndex };
		nextIndex = getMinimumValue(indexes) + 1;
		
		if (nextIndex == 0) {
			nextIndex = selectStatement.length();
		}

		// parsing current clause
		string currentClause = selectStatement.substr(0, nextIndex);
		if (currentClause.find("such that") != -1 || (currentClause.find("and") != -1 && previousClause == "such that")) {
			previousClause = "such that";
			suchThatClauses.push_back(currentClause);
		}
		else if (currentClause.find("pattern") != -1 || (currentClause.find("and") != -1 && previousClause == "pattern")) {
			previousClause = "pattern";
			patternClauses.push_back(currentClause);
		}
		else if (currentClause.find("with") != -1 || (currentClause.find("and") != -1 && previousClause == "with")) {
			previousClause = "with";
			withClauses.push_back(currentClause);
		}

		selectStatement = selectStatement.substr(nextIndex);
	}

	suchThatCondition = splitSuchThatCondition(suchThatClauses);
	patternCondition = splitPatternCondition(patternClauses);

	// TODO::to implement
	// withCondition = splitWithCondition(withClauses);

	// validating 'Select' parameter
	errorString = QueryValidator::validateSelectedVar(selectedVar, declarationsMap);
	if (errorString != "") {
		result.insert("error");
		return result;
	}

	// checking select boolean or synonym
	bool selectBoolean = false;
	if (selectedVar.size() == 1 && selectedVar[0] == "BOOLEAN") {
		selectBoolean = true;
	}

	// validating 'such that' parameter
	errorString = QueryValidator::validateSuchThatParam(suchThatCondition, declarationsMap);
	if (errorString == "semantic error" && selectBoolean) {
		result.insert("false");
		return result;
	}
	else if (errorString != "") {
		result.insert("error");
		return result;
	}

	// validating 'pattern' parameter
	errorString = QueryValidator::validatePatternParam(patternCondition, declarationsMap);
	if (errorString == "semantic error" && selectBoolean) {
		result.insert("false");
		return result;
	}
	else if (errorString != "") {
		result.insert("error");
		return result;
	}

	result = evaluateSelectConditions(declarations, selectedVar, suchThatCondition, patternCondition);

	return result;
}

/*
Splits the query string by char ";".
Returns a vector<string> consisting of every declaration clauses (without trailing whitespaces) and the "Select" statement
*/
vector<string> QueryParser::splitClauses(string query) {
	
	vector<string> output;
	char delimiter = ';';
	int startPoint = 0;
	int endPoint = query.find(delimiter);

	while (endPoint != -1) {
		output.push_back(StringUtil::trim(query.substr(startPoint, endPoint - startPoint), whitespace));
		startPoint = endPoint + 1;
		endPoint = query.find(delimiter, startPoint);
	}

	output.push_back(StringUtil::trim(query.substr(startPoint), whitespace));

	return output;
}

/*
Splits each declarations clause by whitespaces.
Returns a vector<pair<string, string>> consisting of design-entity and synonym
*/
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
				string varNameSplitted = StringUtil::trim(varName.substr(startPoint, endPoint - startPoint), whitespace);
				output.push_back(make_pair(type, varNameSplitted));
				startPoint = endPoint + 1;
				endPoint = varName.find(delimiter, startPoint);
			}

			output.push_back(make_pair(type, StringUtil::trim(varName.substr(startPoint), whitespace)));
		}
		else {
			output.push_back(make_pair(type, StringUtil::trim(varName, whitespace)));
		}
	}

	return output;
}

/*
Splits select clause by whitespaces.
Returns a vector<string> consisting of design-entity selected
*/
vector<string> QueryParser::splitSelectParameter(string selectStatement) {
	
	vector<string> output;

	int firstSpace = selectStatement.find_first_of(whitespace);
	string varName = StringUtil::removeAllWhitespaces(selectStatement.substr(firstSpace));

	if (varName.find("<") != -1 && varName.find(">") != -1) {
		varName = varName.substr(1, varName.length() - 2);

		while (varName.find(",") != -1) {
			int delimiterComma = varName.find(",");
			output.push_back(varName.substr(0, delimiterComma));
			varName = varName.substr(delimiterComma + 1);
		}		
	}
	
	output.push_back(varName);
	
	return output;
}

/*
Splits such that clause by open bracket, comma, and close bracket.
Returns a vector<pair<string, pair<string, string>>> consisting of relation, stmtRef/entRef, stmtRef/entRef
*/
vector<pair<string, pair<string, string>>> QueryParser::splitSuchThatCondition(vector<string> suchThatClause) {
	
	vector<pair<string, pair<string, string>>> output;

	for(int i = 0; i < suchThatClause.size(); i++) {
		int openBracket = suchThatClause[i].find("(");
		int comma = suchThatClause[i].find(",");
		int closeBracket = suchThatClause[i].find(")");
		int strLen = suchThatClause[i].length();

		string condition = StringUtil::removeAllWhitespaces(suchThatClause[i].substr(9, openBracket - 9));
		string firstVar = StringUtil::removeAllWhitespaces(suchThatClause[i].substr(openBracket + 1, comma - openBracket - 1));
		string secondVar = StringUtil::removeAllWhitespaces(suchThatClause[i].substr(comma + 1, closeBracket - comma - 1));

		output.push_back(make_pair(condition, make_pair(firstVar, secondVar)));
	}

	return output;
}

/*
Splits pattern clause by open bracket, comma, and close bracket.
Returns a vector<pair<string, pair<string, string>>> consisting of design-entity, entRef, expression-spec
*/
vector<pair<string, pair<string, string>>> QueryParser::splitPatternCondition(vector<string> patternClause) {
	
	vector<pair<string, pair<string, string>>> output;

	for (int i = 0; i < patternClause.size(); i++) {
		int openBracket = patternClause[i].find("(");
		int comma = patternClause[i].find(",");
		int closeBracket = patternClause[i].find(")");
		int strLen = patternClause[i].length();

		string varName = StringUtil::removeAllWhitespaces(patternClause[i].substr(7, openBracket - 7));
		string firstPattern = StringUtil::removeAllWhitespaces(patternClause[i].substr(openBracket + 1, comma - openBracket - 1));
		string secondPattern = StringUtil::removeAllWhitespaces(patternClause[i].substr(comma + 1, closeBracket - comma - 1));

		output.push_back(make_pair(varName, make_pair(firstPattern, secondPattern)));
	}

	return output;
}

/*
Calls QueryEvaluator to evaluate the query result
Returns a unordered_set<string> consisting of results
*/
unordered_set<string> QueryParser::evaluateSelectConditions(vector<pair<string, string>> declarations, 
	vector<string> selectedVar, vector<pair<string, pair<string, string>>> suchThatCondition,
	vector<pair<string, pair<string, string>>> patternCondition) {

	return QueryEvaluator::evaluateQuery(declarations, selectedVar, suchThatCondition, patternCondition);
}

/*
Get minimum value from the vector args
*/
int QueryParser::getMinimumValue(vector<int> indexes) {

	int minIndex = maxInt;
	for (int i = 0; i < indexes.size(); i++) {
		if (indexes[i] != -1 && indexes[i] < minIndex) {
			minIndex = indexes[i];
		}
	}

	if (minIndex == maxInt) {
		return -1;
	}
	else {
		return minIndex;
	}
}