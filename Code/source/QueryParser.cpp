#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

using namespace std;

#include "QueryParser.h"
#include "LexicalToken.h"

const string whitespace = " \f\n\r\t\v";
const unordered_set<string> validVarType = { "stmt", "read", "print", "call", "while", "if", "assign", "variable", "constant", "procedure" };

string QueryParser::parse(string query) {
	/*
	Main parser function for input query.
	Passes the following to query evaluator:
	1. vector<pair<string, string>> declarations 
	2. vector<string> selectedVar
	3. vector<pair<string, pair<string, string>>> suchThatCondition
	4. vector<pair<string, pair<string, string>>> patternCondition
	*/

	string errorString;

	// initial query validation
	errorString = initialValidation(query);
	if (errorString != "") {
		return "error";
	}

	// splitting clauses
	vector<string> clauses = splitClauses(query);

	// validating clauses
	errorString = validateClauses(clauses);
	if (errorString != "") {
		return "error";
	}

	// parsing declarations
	vector<pair<string, string>> declarations = splitDeclarations(clauses);

	unordered_map<string, string> declarationsMap;
	for (int i = 0; i < declarations.size(); i++) {
		declarationsMap.insert(make_pair(declarations[i].second, declarations[i].first));
	}

	// validating declarations
	errorString = validateDeclarations(declarations);
	if (errorString != "") {
		return "error";
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
	errorString = validateSelectedVar(selectedVar, declarationsMap);
	if (errorString != "") {
		return "error";
	}

	// validating 'such that' parameter
	errorString = validateSuchThatParam(suchThatCondition, declarationsMap);
	if (errorString != "") {
		return "error";
	}

	// validating 'pattern' parameter
	errorString = validatePatternParam(patternCondition, declarationsMap);
	if (errorString != "") {
		return "error";
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
	/*
	Splitting each declarations clause by whitespaces.
	Returns a vector<pair<string, string>> consisting of design-entity and synonym
	*/

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
	/*
	Validate declaration vector based on following conditions:
	- design-entity should be valid
	- synonym should follow the grammar rule: LETTER (LETTER | DIGIT)*
	*/

	for (size_t i = 0; i < declarations.size(); i++) {
		if (validVarType.find(declarations[i].first) == validVarType.end()) {
			return "invalid query";
		}

		if (!LexicalToken::verifyName(declarations[i].second)) {
			return "invalid query";
		}
	}

	return "";
}

vector<string> QueryParser::splitSelectParameter(string selectStatement) {
	/*
	Splitting select clause by whitespaces.
	Returns a vector<string> consisting of design-entity selected
	TODO: implement select boolean and tuples
	*/
	
	vector<string> output;

	int firstSpace = selectStatement.find_first_of(whitespace);
	string varName = removeAllWhitespaces(selectStatement.substr(firstSpace));

	output.push_back(varName);

	return output;
}

vector<pair<string, pair<string, string>>> QueryParser::splitSuchThatCondition(string suchThatClause) {
	/*
	Splitting such that clause by open bracket, comma, and close bracket.
	Returns a vector<pair<string, pair<string, string>>> consisting of relation, stmtRef/entRef, stmtRef/entRef
	*/
	
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
	/*
	Splitting pattern clause by open bracket, comma, and close bracket.
	Returns a vector<pair<string, pair<string, string>>> consisting of design-entity, entRef, expression-spec
	*/
	
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

string QueryParser::validateSelectedVar(vector<string> selectedVar, unordered_map<string, string> declarationsMap) {
	/*
	Validate vector of selected variable based on following conditions:
	- synonym should follow the grammar rule: LETTER (LETTER | DIGIT)*
	- synonym should be declared previously
	*/

	for (int i = 0; i < selectedVar.size(); i++) {
		if (!LexicalToken::verifyName(selectedVar[i])) {
			return "invalid variable name";
		}

		if (declarationsMap.find(selectedVar[i]) == declarationsMap.end()) {
			return "selected variable not found";
		}
	}

	return "";
}

string QueryParser::validateSuchThatParam(vector<pair<string, pair<string, string>>> param, unordered_map<string, string> declarationsMap) {
	/*
	Validate vector of such that parameter based on following conditions:
	- valid relation name
	- for each relation, first and second argument should be valid

	TODO: implement validation for other relations
	*/

	unordered_set<string> validRelation = { "Parent", "Parent*", "Follows", "Follows*", "Uses", "Modifies" };
	unordered_set<string> validArgs = { "stmt", "read", "print", "while", "if", "assign" };
	unordered_set<string> validFirstArgsParent = { "stmt", "while", "if" };
	unordered_set<string> validFirstArgsUses = { "stmt", "print", "while", "if", "procedure", "call" };
	unordered_set<string> validFirstArgsModifies = { "stmt", "read", "while", "if", "procedure", "call" };

	for (int i = 0; i < param.size(); i++) {
		string relation = param[i].first;
		string firstArgs = param[i].second.first;
		string secondArgs = param[i].second.second;
		string firstArgsType;
		string secondArgsType;

		if (validRelation.find(relation) == validRelation.end()) {
			return "invalid query";
		}

		if (declarationsMap.find(firstArgs) != declarationsMap.end()) {
			firstArgsType = declarationsMap.find(firstArgs)->second;
		}

		if (declarationsMap.find(secondArgs) != declarationsMap.end()) {
			secondArgsType = declarationsMap.find(secondArgs)->second;
		}

		if (relation == "Uses") {
			if (firstArgs != "_" && !LexicalToken::verifyInteger(firstArgs) && !LexicalToken::verifyName(firstArgs) &&
				validFirstArgsUses.find(firstArgsType) == validFirstArgsUses.end()) {
				return "invalid query";
			}

			if (secondArgs != "_" && !LexicalToken::verifyInteger(secondArgs) && !LexicalToken::verifyName(secondArgs)) {
				return "invalid query";
			}
		}
		else if (relation == "Modifies") {
			if (firstArgs != "_" && !LexicalToken::verifyInteger(firstArgs) && !LexicalToken::verifyName(firstArgs) &&
				validFirstArgsModifies.find(firstArgsType) == validFirstArgsModifies.end()) {
				return "invalid query";
			}

			if (secondArgs != "_" && !LexicalToken::verifyInteger(secondArgs) && !LexicalToken::verifyName(secondArgs)) {
				return "invalid query";
			}
		}
		else if (relation == "Parent" || relation == "Parent*") {
			if (firstArgs != "_" && !LexicalToken::verifyInteger(firstArgs) &&
				validFirstArgsParent.find(firstArgsType) == validFirstArgsParent.end()) {
				return "invalid query";
			}

			if (secondArgs != "_" && !LexicalToken::verifyInteger(secondArgs) && 
				validArgs.find(secondArgsType) == validArgs.end()) {
				return "invalid query";
			}
		}
		else {
			// relation == Follows | Follows*

			if (firstArgs != "_" && !LexicalToken::verifyInteger(firstArgs) &&
				validArgs.find(firstArgsType) == validArgs.end()) {
				return "invalid query";
			}

			if (secondArgs != "_" && !LexicalToken::verifyInteger(secondArgs) &&
				validArgs.find(secondArgsType) == validArgs.end()) {
				return "invalid query";
			}
		}
	}

	return "";
}

string QueryParser::validatePatternParam(vector<pair<string, pair<string, string>>> param, unordered_map<string, string> declarationsMap) {
	/*
	Validate vector of pattern parameter based on following conditions:
	- synonym should follow the grammar rule: LETTER (LETTER | DIGIT)*
	- synonym should be declared previously
	- synonym should be assign type (assign-synonym)

	TODO: implement validation for if and while pattern
	*/


	for (int i = 0; i < param.size(); i++) {
		string stmt = param[i].first;

		if (!LexicalToken::verifyName(stmt)) {
			return "invalid variable name";
		}

		if (declarationsMap.find(stmt) == declarationsMap.end()) {
			return "statement not found";
		}
		else if ((declarationsMap.find(stmt))->second != "assign") {
			return "statement type is not assign";
		}
	}

	return "";
}

string QueryParser::evaluateSelectConditions(vector<pair<string, string>> declarations, 
	vector<string> selectedVar, vector<pair<string, pair<string, string>>> suchThatCondition,
	vector<pair<string, pair<string, string>>> patternCondition) {
	
	// TODO: call query evaluator

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
