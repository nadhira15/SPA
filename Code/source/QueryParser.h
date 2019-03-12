#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class QueryParser {
public:
	static unordered_set<string> parse(string query);

	static vector<string> splitClauses(string query);
	static vector<pair<string, string>> splitDeclarations(vector<string> clauses);
	static vector<string> splitSelectParameter(string selectStatement);
	static vector<pair<string, pair<string, string>>> splitSuchThatCondition(string suchThatClause);
	static vector<pair<string, pair<string, string>>> splitPatternCondition(string patternClause);
	static unordered_set<string> evaluateSelectConditions(vector<pair<string, string>> declarations,
		vector<string> selectedVar, vector<pair<string, pair<string, string>>> suchThatCondition,
		vector<pair<string, pair<string, string>>> patternCondition);
};