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
	static vector<pair<string, string>> splitSelectConditions(string selectStatement);
	static string validateSelectConditions(vector<pair<string, string>> selectConditions);
	static string evaluateSelectConditions(vector<pair<string, string>> declarations, vector<pair<string, string>> selectConditions);
};