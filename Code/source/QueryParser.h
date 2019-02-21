#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class QueryParser {
public:
	static string parse(string query);

	static string initialValidation(string query);
	static vector<string> splitClauses(string query);
	static string validateClauses(vector<string> clauses);
	static vector<pair<string, string>> splitDeclarations(vector<string> clauses);
	static string validateDeclarations(vector<pair<string, string>> declarations);
	static vector<string> splitSelectParameter(string selectStatement);
	static vector<pair<string, pair<string, string>>> splitSuchThatCondition(string suchThatClause);
	static vector<pair<string, pair<string, string>>> splitPatternCondition(string patternClause);
	static string validateSelectedVar(vector<string> selectedVar, vector<pair<string, string>> declarations);
	static string validateSuchThatParam(vector<pair<string, pair<string, string>>> param);
	static string validatePatternParam(vector<pair<string, pair<string, string>>> param);
	static string evaluateSelectConditions(vector<pair<string, string>> declarations,
		vector<string> selectedVar, vector<pair<string, pair<string, string>>> suchThatCondition,
		vector<pair<string, pair<string, string>>> patternCondition);
	static string removeAllWhitespaces(string s);
	static string removeTrailingWhitespaces(string s);
};