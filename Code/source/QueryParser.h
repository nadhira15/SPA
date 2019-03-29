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
	static unordered_map<string, string> splitDeclarations(vector<string> clauses);
	static vector<string> splitSelectParameter(string selectStatement);
	static vector<pair<string, pair<string, string>>> splitSuchThatCondition(vector<string> suchThatClause);
	static vector<pair<string, pair<string, string>>> splitPatternCondition(vector<string> patternClause);
	static vector<pair<string, string>> splitWithCondition(vector<string> withClause);
	static unordered_set<string> evaluateSelectConditions(
		unordered_map<string, string> declarations,
		vector<string> selectedVar, 
		vector<pair<string, pair<string, string>>> suchThatCondition,
		vector<pair<string, pair<string, string>>> patternCondition,
		vector<pair<string, string>> withCondition);

	static int getMinimumValue(vector<int> indexes);
};