#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"

class QueryEvaluator
{
public:
	static unordered_set<string> evaluateQuery(vector<pair<string, string>> declarations,
		vector<string> selectedVar, vector<pair<string, pair<string, string>>> suchThatCondition,
		vector<pair<string, pair<string, string>>> patternCondition);

	static std::unordered_map<std::string, std::vector<std::string>> evaluatePatternCondition(vector<pair<string,
		pair<string, string>>> patternCondition);
	static unordered_set<string> filterSuchThatCondition(vector<pair<string, string>> declarations, 
		vector<string> selectedVar,
		vector<pair<string, pair<string, string>>> suchThatCondition, 
		vector<pair<string, pair<string, string>>> patternCondition);

	static string isSuchThatTrivial(string relation, string firstArgument, string secondArgument);
	static unordered_set<string> evaluateSuchThat(string relation, string firstArgument, string secondArgument);

	static unordered_set<string> getAllStms();
	static unordered_set<string> getStmts(string s);
	static unordered_set<string> filterType(string typeRequired, unordered_set<string>toBeFiltered);
	static unordered_set<string> filterType(string firstTypeRequired, string secondTypeRequired,
		unordered_set<string> toBeFiltered);
	static unordered_set<string> QueryEvaluator::getAssign(unordered_set<string> afterPatternFilter);

	static string truthValue(bool boolean);
	static string trimFrontEnd(string quotedString);

	static bool isInteger(string s);
	static bool isQuoted(string s);
	static bool isSynonym(string s);

};