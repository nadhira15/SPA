#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class QueryValidator {
public:

	static string initialValidation(string query);
	static string validateClauses(vector<string> clauses);
	static string validateDeclarations(unordered_map<string, string> declarations);
	static string validateSelectedVar(vector<string> selectedVar, unordered_map<string, string> declarationsMap);
	static string validateSuchThatParam(vector<pair<string, pair<string, string>>> param, unordered_map<string, string> declarationsMap);
	static string validatePatternParam(vector<pair<string, pair<string, string>>> param, unordered_map<string, string> declarationsMap);
	static string validateWithParam(vector<pair<string, string>> param, unordered_map<string, string> declarationsMap);
};