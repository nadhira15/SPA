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
	if (patternCondition.size == 0) {
		return setToString(filterSuchThatCondition(declarations, selectedVar, suchThatCondition, getAllStms(), ""));
	}
	unordered_set<int> afterPatternFilter = filterPatternCondition(patternCondition);
	string patternSynonym = patternCondition[0].first;

	unordered_set<string> afterSuchThatFilter = filterSuchThatCondition(declarations, selectedVar, 
		suchThatCondition, afterPatternFilter, patternSynonym);
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
	vector<pair<string, pair<string, string>>> suchThatCondition, unordered_set<int> afterPatternFilter, string patternSynonym) {
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
		if ((selectedVarType != "assign") || (patternSynonym.size == 0)) {
			return getStmts(selectedVarType);
		}
		return intSetToStrSet(afterPatternFilter);
	}
	else if (certainty == "not trivial") {
		if (patternSynonym.size == 0) {
			
		}
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

unordered_set<string> getStmts(string s) {
	unordered_set<string> result;
	if (s == "stmt") {
		return intSetToStrSet(getAllStms());
	}
	else if (s == "read") {
		return intSetToStrSet(PKB().getReadStms());
	}
	else if (s == "print") {
		return intSetToStrSet(PKB().getPrintStms());
	}
	else if (s == "while") {
		return intSetToStrSet(PKB().getWhileStms());
	}
	else if (s == "if") {
		return intSetToStrSet(PKB().getIfStms());
	}
	else if (s == "assign") {
		return intSetToStrSet(PKB().getAssignStms());
	}
	else if (s == "variable") {
		return PKB().getVariables();
	}
	else if (s == "constant") {
		return intSetToStrSet(PKB().getConstants());
	}
	else if (s == "procedure") {
		result.insert(PKB().getProcName());
	}
	return result;
}

unordered_set<string> filterTypeNonTuple(string typeRequired, unordered_set<string>toBeFiltered) {
	unordered_set<string> typeRequiredSet = getStmts(typeRequired);
	unordered_set<string> filteredSet;
	for (unordered_set<string>::iterator it = toBeFiltered.begin(); it != toBeFiltered.end(); ++it) {
		string pointer = *it;
		if (typeRequiredSet.count(pointer) == 1) {
			filteredSet.insert(pointer);
		}
	}

	return filteredSet;
}

unordered_set<pair<string, string>> filterTypeTuple(string firstTypeRequired, string secondTypeRequired,
	unordered_set<pair<string, string>> toBeFiltered) {
	unordered_set<string> firstTypeRequiredSet = getStmts(firstTypeRequired);
	unordered_set<string> secondTypeRequiredSet = getStmts(secondTypeRequired);
	unordered_set<pair<string, string>> filteredSet;
	for (unordered_set<pair<string, string>>::iterator it = toBeFiltered.begin(); it != toBeFiltered.end(); ++it) {
		pair<string, string> pointer = *it;
		if (firstTypeRequiredSet.count(pointer.first) == 1 &&
			secondTypeRequiredSet.count(pointer.second) == 1) {
			filteredSet.insert(pointer);
		}
	}
}



