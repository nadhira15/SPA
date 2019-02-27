#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <algorithm>

using namespace std;

#include "PKB.h"
#include "QueryEvaluator.h"
#include "LexicalToken.h"

string QueryEvaluator::evaluateQuery(vector<pair<string, string>> declarations,
	vector<string> selectedVar, vector<pair<string, pair<string, string>>> suchThatCondition,
	vector<pair<string, pair<string, string>>> patternCondition) {
	
	unordered_set<int> afterPatternFilter = filterPatternCondition(patternCondition);

	if (suchThatCondition.size == 0) {
		return setToString(intSetToStrSet(afterPatternFilter));
	}

	unordered_set<string> afterSuchThatFilter = filterSuchThatCondition(declarations, selectedVar, suchThatCondition, afterPatternFilter);
	return setToString(afterSuchThatFilter);
}

unordered_set<int> filterPatternCondition(vector<pair<string, pair<string, string>>> patternCondition) {
	if (patternCondition.size == 0) {
		return getAllStms();
	}
}

/*
Evaluate the such that condition which returns a list of answers
*/
unordered_set<string> filterSuchThatCondition(vector<pair<string, string>> declarations, vector<string> selectedVar,
	vector<pair<string, pair<string, string>>> suchThatCondition, unordered_set<int> afterPatternFilter) {
	string relation = suchThatCondition[0].first;
	string firstArgument = suchThatCondition[0].second.first;
	string secondArgument = suchThatCondition[0].second.second;
	string selectedVarType;
	string firstArgumentType;
	string secondArgumentType;
	for (vector<pair<string, string>>::size_type i = 0; i != declarations.size(); i++) {
		if (declarations[i].second == selectedVar[0]) {
			selectedVarType = declarations[i].first;
		}

		if (declarations[i].second == firstArgument) {
			firstArgumentType = declarations[i].first;
		}

		if (declarations[i].second == secondArgument) {
			secondArgumentType = declarations[i].first;
		}
	}

	unordered_set<string> evaluation;
	string certainty = isSuchThatTrivial(relation, firstArgument, secondArgument);

	if (certainty == "false") {
		evaluation.insert("none");
	}
	else if (certainty == "true") {
		if ((afterPatternFilter.size == PKB().getTotalStmNo()) || selectedVarType != "assign") {
			if (selectedVarType == "stmt") {
				return intSetToStrSet(getAllStms());
			}
			else if (selectedVarType == "read") {
				return intSetToStrSet(PKB().getReadStms());
			}
			else if (selectedVarType == "print") {
				return intSetToStrSet(PKB().getPrintStms());
			}
			else if (selectedVarType == "while") {
				return intSetToStrSet(PKB().getWhileStms());
			}
			else if (selectedVarType == "if") {
				return intSetToStrSet(PKB().getIfStms());
			}
			else if (selectedVarType == "variable") {
				return PKB().getVariables();
			}
			else if (selectedVarType == "constant") {
				return intSetToStrSet(PKB().getConstants());
			}
			else if (selectedVarType == "procedure") {
				evaluation.insert(PKB().getProcName());
			}
			return evaluation;
		}
		return intSetToStrSet(afterPatternFilter);
	}
}

/*
Evaluate the such that conditions which gives boolean answer
*/
string isSuchThatTrivial(string relation, string firstArgument, string secondArgument) {
	if (relation == "Follows") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasFollowRelation());
			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				return truthValue(PKB().getPrvStm(stoi(secondArgument)) > 0);
			}
			return "not trivial";
		}
		else if (LexicalToken::verifyInteger(firstArgument)) {
			if (secondArgument == "_") {
				return truthValue(PKB().getNxtStm(stoi(firstArgument)) > 0);
 			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				return truthValue(PKB().getNxtStm(stoi(firstArgument)) == stoi(secondArgument));
			}
			return "not trivial";
		}
		return "not trivial";
	}
	else if (relation == "Follows*") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasFollowRelation());
			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				return truthValue(PKB().getPrvStm(stoi(secondArgument)) > 0);
			}
			return "not trivial";
		}
		else if (LexicalToken::verifyInteger(firstArgument)) {
			if (secondArgument == "_") {
				return truthValue(PKB().getNxtStm(stoi(firstArgument)) > 0);
			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				return truthValue(PKB().hasFollow_S_Pair(stoi(firstArgument), stoi(secondArgument)));
			}
			return "not trivial";
		}
		return "not trivial";
	}
	else if (relation == "Parent") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasParentRelation());
			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				return truthValue(PKB().isChild(stoi(secondArgument)) > 0);
			}
			return "not trivial";
		}
		else if (LexicalToken::verifyInteger(firstArgument)) {
			if (secondArgument == "_") {
				return truthValue(PKB().isParent(stoi(firstArgument)) > 0);
			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				return truthValue(PKB().getParent(stoi(firstArgument)) == stoi(secondArgument));
			}
			return "not trivial";
		}
		return "not trivial";
	}
	else if (relation == "Parent*") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasParentRelation());
			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				return truthValue(PKB().isChild(stoi(secondArgument)) > 0);
			}
			return "not trivial";
		}
		else if (LexicalToken::verifyInteger(firstArgument)) {
			if (secondArgument == "_") {
				return truthValue(PKB().isParent(stoi(firstArgument)) > 0);
			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				return truthValue(PKB().hasAncDescPair(stoi(firstArgument), stoi(secondArgument)));
			}
			return "not trivial";
		}
		return "not trivial";
	}
	else if (relation == "Uses") {
		if (LexicalToken::verifyInteger(firstArgument)) {
			if (secondArgument.front == "\"") {
				return truthValue(PKB().isUsing(stoi(firstArgument), secondArgument));
			}
			else if (secondArgument == "_") {
				return PKB().getUsedVar(stoi(firstArgument)).size > 0;
			}
			return "not trivial";
		}
		else if (firstArgument.front == "\"") {
			if (secondArgument.front == "\"") {
				return truthValue(PKB().isUsing(firstArgument, secondArgument));
			}
			else if (secondArgument == "_") {
				return PKB().getUsedVar(firstArgument).size > 0;;
			}
			return "not trivial";
		}
		return "not trivial";
	}
	else if (relation == "Modifies") {
		if (LexicalToken::verifyInteger(firstArgument)) {
			if (secondArgument.front == "\"") {
				return truthValue(PKB().isModifying(stoi(firstArgument), secondArgument));
			}
			else if (secondArgument == "_") {
				return truthValue(PKB().getModifiedVar(stoi(firstArgument)) != "");
			}
			return "not trivial";
		}
		else if (firstArgument.front == "\"") {
			if (secondArgument.front == "\"") {
				return truthValue(PKB().isModifying(firstArgument, secondArgument));
			}
			else if (secondArgument == "_") {
				return PKB().getModifiedVar(firstArgument).size != "";
			}
			return "not trivial";
		}
		return "not trivial";
	}

	return "not trivial";
}

unordered_set<int> getAllStms() {
	unordered_set<int> allStms;
	for (int i = 1; i <= PKB().getTotalStmNo(); i++) {
		allStms.insert(i);
	}

	return allStms;
}
string setToString(unordered_set<string> setOfString) {
	stringstream ss;
	for (unordered_set<string>::iterator it = setOfString.begin(); it != setOfString.end(); ++it) {
		ss << *it;
		ss << ", ";
	}

	string s = ss.str();
	return s;
}

unordered_set<string> intSetToStrSet(unordered_set<int> intList) {
	unordered_set<string> strList;
	for (unordered_set<int>::iterator it = intList.begin(); it != intList.end(); ++it) {
		strList.insert(to_string(*it));
	}

	return strList;
}

string truthValue(bool boolean) {
	if (boolean) {
		return "true";
	}

	return "false";
}

unordered_set<string> intPairSetToStrSet(unordered_set<pair<int, int>> intPairSet) {
	unordered_set<string> strSet;
	for (unordered_set<pair<int, int>>::iterator it = intPairSet.begin(); it != intPairSet.end(); ++it) {
		pair<int, int> pointer = *it;
		strSet.insert(to_string(pointer.first) + " " + to_string(pointer.second));
	}

	return strSet;
}



