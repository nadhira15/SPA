#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

#include "QueryEvaluator.h"
#include "QueryParser.h"
#include "QueryValidator.h"
#include "StringUtil.h"

string whitespace = " \f\n\r\t\v";

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

	//TODO: implement parser for more than 1 such that clause and pattern

	int suchThatIndex = selectStatement.find("such that");
	int patternIndex = selectStatement.find("pattern");
	int selectStatementLen = selectStatement.length();

	if (suchThatIndex == -1 && patternIndex == -1) {
		selectedVar = splitSelectParameter(selectStatement);
	}
	else if (suchThatIndex != -1 && patternIndex == -1) {
		selectedVar = splitSelectParameter(selectStatement.substr(0, suchThatIndex));
		suchThatCondition = splitSuchThatCondition(selectStatement.substr(suchThatIndex));
	}
	else if (suchThatIndex == -1 && patternIndex != -1) {
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
TODO: implement select boolean and tuples
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
vector<pair<string, pair<string, string>>> QueryParser::splitSuchThatCondition(string suchThatClause) {
	
	vector<pair<string, pair<string, string>>> output;

	int openBracket = suchThatClause.find("(");
	int comma = suchThatClause.find(",");
	int closeBracket = suchThatClause.find(")");
	int strLen = suchThatClause.length();

	string condition = StringUtil::removeAllWhitespaces(suchThatClause.substr(9, openBracket - 9));
	string firstVar = StringUtil::removeAllWhitespaces(suchThatClause.substr(openBracket + 1, comma - openBracket - 1));
	string secondVar = StringUtil::removeAllWhitespaces(suchThatClause.substr(comma + 1, closeBracket - comma - 1));

	output.push_back(make_pair(condition, make_pair(firstVar, secondVar)));

	return output;
}

/*
Splits pattern clause by open bracket, comma, and close bracket.
Returns a vector<pair<string, pair<string, string>>> consisting of design-entity, entRef, expression-spec
*/
vector<pair<string, pair<string, string>>> QueryParser::splitPatternCondition(string patternClause) {
	
	vector<pair<string, pair<string, string>>> output;

	int openBracket = patternClause.find("(");
	int comma = patternClause.find(",");
	int closeBracket = patternClause.find(")");
	int strLen = patternClause.length();

	string varName = StringUtil::removeAllWhitespaces(patternClause.substr(7, openBracket - 7));
	string firstPattern = StringUtil::removeAllWhitespaces(patternClause.substr(openBracket + 1, comma - openBracket - 1));
	string secondPattern = StringUtil::removeAllWhitespaces(patternClause.substr(comma + 1, closeBracket - comma - 1));

	output.push_back(make_pair(varName, make_pair(firstPattern, secondPattern)));

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