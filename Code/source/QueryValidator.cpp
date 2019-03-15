#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

#include "QueryValidator.h"
#include "LexicalToken.h"
#include "ExpressionUtil.h"

const unordered_set<string> validVarType = { "stmt", "read", "print", "call", "while", "if", "assign", "variable", "constant", "prog_line", "procedure" };

/*
Checks whether the input string is valid:
- has length more than 0
- has substring "Select"
- has declaration
Output: error message (if any), otherwise empty string
*/
string QueryValidator::initialValidation(string query) {

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

/*
Validates declaration clauses based on following conditions:
- "Select" clause appears after all declaration clauses
- Each declaration clause consists of varType and varName
*/
string QueryValidator::validateClauses(vector<string> clauses) {
	
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

/*
Validates declaration vector based on following conditions:
- design-entity should be valid
- synonym should follow the grammar rule: LETTER (LETTER | DIGIT)*
*/
string QueryValidator::validateDeclarations(vector<pair<string, string>> declarations) {
	
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

/*
Validates vector of selected synonym based on following conditions:
- synonym should follow the grammar rule: LETTER (LETTER | DIGIT)*
- synonym should be declared previously
*/
string QueryValidator::validateSelectedVar(vector<string> selectedVar, unordered_map<string, string> declarationsMap) {
	
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

/*
Validates vector of such that parameter based on following conditions:
- valid relation name
- for each relation, first and second argument should be valid

TODO: implement validation for other relations
*/
string QueryValidator::validateSuchThatParam(vector<pair<string, pair<string, string>>> param, unordered_map<string, string> declarationsMap) {

	unordered_set<string> validRelation = { "Parent", "Parent*", "Follows", "Follows*", "Uses", "Modifies" };
	unordered_set<string> validArgs = { "stmt", "read", "print", "while", "if", "assign", "call" };
	unordered_set<string> validFirstArgsParent = { "stmt", "while", "if" };
	unordered_set<string> validFirstArgsUses = { "stmt", "print", "while", "if", "procedure", "assign", "call" };
	unordered_set<string> validFirstArgsModifies = { "stmt", "read", "while", "if", "procedure", "assign", "call" };

	for (int i = 0; i < param.size(); i++) {
		string relation = param[i].first;
		string firstArgs = param[i].second.first;
		string secondArgs = param[i].second.second;
		string firstArgsType;
		string secondArgsType;

		// Initial Validation
		if (validRelation.find(relation) == validRelation.end()) {
			return "invalid query";
		}

		if (declarationsMap.find(firstArgs) != declarationsMap.end()) {
			firstArgsType = declarationsMap.find(firstArgs)->second;
		}

		if (declarationsMap.find(secondArgs) != declarationsMap.end()) {
			secondArgsType = declarationsMap.find(secondArgs)->second;
		}

		// Switch Cases according to relation
		if (relation == "Parent" || relation == "Parent*") {
			
			// Validating first args
			if (firstArgs == "_" ||
				LexicalToken::verifyInteger(firstArgs) ||
				validFirstArgsParent.find(firstArgsType) != validFirstArgsParent.end()) {
				// valid first args
			}
			else {
				return "invalid query";
			}

			// Validating second args
			if (secondArgs == "_" ||
				LexicalToken::verifyInteger(secondArgs) ||
				validArgs.find(secondArgsType) != validArgs.end()) {
				// valid second args
			}
			else {
				return "invalid query";
			}
		} 
		else if (relation == "Follows" || relation == "Follows*") {

			// Validating first args
			if (firstArgs == "_" ||
				LexicalToken::verifyInteger(firstArgs) ||
				validArgs.find(firstArgsType) != validArgs.end()) {
				// valid first args
			}
			else {
				return "invalid query";
			}

			// Validating second args
			if (secondArgs == "_" ||
				LexicalToken::verifyInteger(secondArgs) ||
				validArgs.find(secondArgsType) != validArgs.end()) {
				// valid second args
			}
			else {
				return "invalid query";
			}
		} 
		else if (relation == "Uses") {

			// Validating first args
			if (LexicalToken::verifyInteger(firstArgs) ||
				(firstArgs[0] == '"' && LexicalToken::verifyName(firstArgs.substr(1, firstArgs.length() - 2))) ||
				(firstArgs[0] != '"' && LexicalToken::verifyName(firstArgs) && (validFirstArgsUses.find(firstArgsType) != validFirstArgsUses.end()))) {
				// valid first args
			}
			else {
				return "invalid query";
			}

			// Validating second args
			if (secondArgs == "_" ||
				LexicalToken::verifyInteger(secondArgs) ||
				(secondArgs[0] == '"' && LexicalToken::verifyName(secondArgs.substr(1, secondArgs.length() - 2))) ||
				(secondArgs[0] != '"' && LexicalToken::verifyName(secondArgs))) {
				// valid second args
			}
			else {
				return "invalid query";
			}
		}
		else if (relation == "Modifies") {

			// Validating first args
			if (LexicalToken::verifyInteger(firstArgs) ||
				(firstArgs[0] == '"' && LexicalToken::verifyName(firstArgs.substr(1, firstArgs.length() - 2))) ||
				(firstArgs[0] != '"' && LexicalToken::verifyName(firstArgs) && (validFirstArgsModifies.find(firstArgsType) != validFirstArgsModifies.end()))) {
				// valid first args
			}
			else {
				return "invalid query";
			}

			// Validating second args
			if (secondArgs == "_" ||
				LexicalToken::verifyInteger(secondArgs) ||
				(secondArgs[0] == '"' && LexicalToken::verifyName(secondArgs.substr(1, secondArgs.length() - 2))) ||
				(secondArgs[0] != '"' && LexicalToken::verifyName(secondArgs))) {
				// valid first args
			}
			else {
				return "invalid query";
			}
		}
	}

	return "";
}

/*
Validates vector of pattern parameter based on following conditions:
- synonym should follow the grammar rule: LETTER (LETTER | DIGIT)*
- synonym should be declared previously
- synonym should be assign type (assign-synonym)
- for each pattern clause, first and second argument should be valid

TODO: implement validation for if and while pattern
*/
string QueryValidator::validatePatternParam(vector<pair<string, pair<string, string>>> param, unordered_map<string, string> declarationsMap) {
	
	for (int i = 0; i < param.size(); i++) {
		string stmt = param[i].first;
		string firstArgs = param[i].second.first;
		string secondArgs = param[i].second.second;

		// Assign synonym validation
		if (!LexicalToken::verifyName(stmt)) {
			return "invalid assign synonym";
		}

		if (declarationsMap.find(stmt) == declarationsMap.end()) {
			return "statement not found";
		}
		else if ((declarationsMap.find(stmt))->second != "assign") {
			return "statement type is not assign";
		}

		// First argument validation
		if (firstArgs == "_" ||
			(firstArgs[0] != '"' && (declarationsMap.find(firstArgs) != declarationsMap.end()) && ((declarationsMap.find(firstArgs))->second == "variable")) ||
			(firstArgs[0] == '"' && LexicalToken::verifyName(firstArgs.substr(1, firstArgs.length() - 2)))) {
			// valid first args
		}
		else {
			return "invalid first args";
		}

		// Second argument validation
		if (secondArgs == "_" ||
			(secondArgs[0] == '_' && ExpressionUtil::verifyInfixExpression(secondArgs.substr(2, secondArgs.length() - 4))) ||
			(secondArgs[0] == '"' && ExpressionUtil::verifyInfixExpression(secondArgs.substr(1, secondArgs.length() - 2)))) {
			// valid second args
		}
		else {
			return "invalid second args";
		}

	}

	return "";
}