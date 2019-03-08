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
	static unordered_set<string> filterPatternCondition(vector<pair<string, pair<string, string>>> patternCondition);
	static unordered_set<string> filterSuchThatCondition(vector<pair<string, string>> declarations, vector<string> selectedVar,
		vector<pair<string, pair<string, string>>> suchThatCondition, vector<pair<string, pair<string, string>>> patternCondition);
	static string isSuchThatTrivial(string relation, string firstArgument, string secondArgument);
	static unordered_set<string> evaluateSuchThat(string relation, string firstArgument, string secondArgument);
	static unordered_set<string> getAllStms();
	static unordered_set<string> intVecToStrSet(vector<int> intVec);
	static unordered_set<string> intStrVecToStrSet(vector<pair<int, string>> intVec);
	static unordered_set<string> intSetToStrSet(unordered_set<int> intSet);
	static string truthValue(bool boolean);
	static unordered_set<string> intPairSetToStrSet(unordered_set<pair<int, int>, intPairhash> intPairSet);
	static unordered_set<string> intStrSetToStrSet(unordered_set<pair<int, string>, intStringhash> intStringSet);
	static unordered_set<string> strPairSetToStrSet(unordered_set<pair<string, string>, strPairhash> strPairSet);
	static unordered_set<string> getStmts(string s);
	static unordered_set<string> filterType(string typeRequired, unordered_set<string>toBeFiltered);
	static unordered_set<string> filterType(string firstTypeRequired, string secondTypeRequired,
		unordered_set<string> toBeFiltered);
	static unordered_set<string> getFirstParam(unordered_set<string> stringPair);
	static unordered_set<string> getSecondParam(unordered_set<string> stringPair);
	static unordered_set<string> intersection(unordered_set<string> first, unordered_set<string> toContain);
	static unordered_set<string> getOtherPair(int position, unordered_set<string> stringPair, unordered_set<string> toContain);
	static unordered_set<string> QueryEvaluator::getAssign(unordered_set<string> afterPatternFilter);
	static string trimFrontEnd(string quotedString);

};