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
#include "Hasher.h"

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

	unordered_set<int> result;
	vector<int> PKBresult;

	for (int i = 0; i < patternCondition.size(); i++) {
		string LHSpattern = patternCondition[i].second.first;
		string RHSpattern = patternCondition[i].second.second;
		bool isExclusive = true;
		string variable, expr;

		if (RHSpattern[0] == '_' && RHSpattern[RHSpattern.length() - 1] == '_') {
			isExclusive = false;
		}

		if (LHSpattern[0] == '_') {
			PKBresult = PKB().findPattern(RHSpattern, isExclusive);
		}
		else {
			PKBresult = PKB().findPattern(LHSpattern, RHSpattern, isExclusive);
		}
	}

	for (int i = 0; i < PKBresult.size(); i++) {
		result.insert(PKBresult[i]);
	}

	return result;
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

	if (firstArgumentType == "procedure") {
		firstArgument = PKB().getProcName();
	}
	unordered_set<string> evaluation;
	string certainty = isSuchThatTrivial(relation, firstArgument, secondArgument);

	if (certainty == "false") {
		evaluation.insert("none");
	}
	else if ((certainty == "true") || (suchThatCondition.size == 0)) {
		if ((selectedVar[0] != patternSynonym) || (patternSynonym.size == 0)) {
			return getStmts(selectedVarType);
		}
		return intSetToStrSet(afterPatternFilter);
	}
	else if (certainty == "not trivial") {
		unordered_set<string> suchThatResult = evaluateSuchThat(relation, firstArgument, secondArgument);
		if (patternSynonym.size == 0) {
			if (selectedVar[0] == firstArgument) {
				if (secondArgumentType == "") {
					return filterTypeNonTuple(firstArgumentType, suchThatResult);
				} 
				return getFirstParam(evaluateSuchThat(relation, firstArgument, secondArgument));
			}
			if (selectedVar[0] == secondArgument) {
				if (firstArgumentType == "") {
					return filterTypeNonTuple(secondArgumentType, suchThatResult);
				}
				return getSecondParam(evaluateSuchThat(relation, firstArgument, secondArgument));
			}
			if (suchThatResult.size == 0) {
				evaluation.insert("none");
			}
			return getStmts(selectedVarType);
		}
		if (selectedVar[0] != patternSynonym && 
			firstArgument != patternSynonym &&
			secondArgument != patternSynonym) {
			if (afterPatternFilter.size == 0) {
				evaluation.insert("none");
			}
			return filterSuchThatCondition(declarations, selectedVar, suchThatCondition, getAllStms(), "");
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

unordered_set<string> evaluateSuchThat(string relation, string firstArgument, string secondArgument) {
	unordered_set<string> result;
	if (relation == "Follows") {
		if (firstArgument != "_" && !LexicalToken::verifyInteger(firstArgument)) {
			if (secondArgument == "_") {
				return intSetToStrSet(PKB().getAllFollowed());
			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				result.insert(to_string(PKB().getPrvStm(stoi(secondArgument))));
			}
			else {
				return intPairSetToStrSet(PKB().getFollowPairs());
			}
		}
		if (secondArgument != "_" && !LexicalToken::verifyInteger(secondArgument)) {
			if (firstArgument == "_") {
				return intSetToStrSet(PKB().getAllFollowers());
			}
			else if (LexicalToken::verifyInteger(firstArgument)) {
				result.insert(to_string(PKB().getNxtStm(stoi(secondArgument))));
			}
			else {
				return result;
			}
		}
		return result;
	}
	else if (relation == "Follows*") {
		if (firstArgument != "_" && !LexicalToken::verifyInteger(firstArgument)) {
			if (secondArgument == "_") {
				return intSetToStrSet(PKB().getAllFollowed());
			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				return intSetToStrSet(PKB().getAllFollowedBy(stoi(secondArgument)));
			}
			else {
				return intPairSetToStrSet(PKB().getFollow_S_Pairs());
			}
		}
		if (secondArgument != "_" && !LexicalToken::verifyInteger(secondArgument)) {
			if (firstArgument == "_") {
				return intSetToStrSet(PKB().getAllFollowers());
			}
			else if (LexicalToken::verifyInteger(firstArgument)) {
				return intSetToStrSet(PKB().getAllFollowing(stoi(secondArgument)));
			}
			else {
				return result;
			}
		}
		return result;
	}
	if (relation == "Parent") {
		if (firstArgument != "_" && !LexicalToken::verifyInteger(firstArgument)) {
			if (secondArgument == "_") {
				return intSetToStrSet(PKB().getAllParents());
			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				result.insert(to_string(PKB().getParent(stoi(secondArgument))));
			}
			else {
				return intPairSetToStrSet(PKB().getParentChildPairs());
			}
		}
		if (secondArgument != "_" && !LexicalToken::verifyInteger(secondArgument)) {
			if (firstArgument == "_") {
				return intSetToStrSet(PKB().getAllChildren());
			}
			else if (LexicalToken::verifyInteger(firstArgument)) {
				return intSetToStrSet(PKB().getChildren(stoi(secondArgument)));
			}
			else {
				return result;
			}
		}
		return result;
	}
	else if (relation == "Parent*") {
		if (firstArgument != "_" && !LexicalToken::verifyInteger(firstArgument)) {
			if (secondArgument == "_") {
				return intSetToStrSet(PKB().getAllParents());
			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				return intSetToStrSet(PKB().getAllAncestors(stoi(secondArgument)));
			}
			else {
				return intPairSetToStrSet(PKB().getAncDescPairs());
			}
		}
		if (secondArgument != "_" && !LexicalToken::verifyInteger(secondArgument)) {
			if (firstArgument == "_") {
				return intSetToStrSet(PKB().getAllParents());
			}
			else if (LexicalToken::verifyInteger(firstArgument)) {
				return intSetToStrSet(PKB().getAllDescendants(stoi(secondArgument)));
			}
			else {
				return result;
			}
		}
		return result;
	}
	else if (relation == "Uses") {
		if ((secondArgument != "_") && (secondArgument.front != "\"")) {
			if ((LexicalToken::verifyInteger(firstArgument)) || firstArgument.front == "\"") {
				return PKB().getUsedVar(firstArgument);
			} 
			if (firstArgument == PKB().getProcName()) {
				return strPairSetToStrSet(PKB().getProcVarUsePairs());
			}
			return intStrSetToStrSet(PKB().getStmVarUsePairs());
		}
		if ((firstArgument != "_") && (secondArgument.front != "\"")) {
			if (secondArgument == "_") {
				return intSetToStrSet(PKB().getStmUsing(""));
			}
			else if (secondArgument.front == "\"") {
				return intSetToStrSet(PKB().getStmUsing(secondArgument));
			} 
			else {
				return result;
			}
		}
		return result;
	}
	else if (relation == "Modifies") {
		if ((secondArgument != "_") && (secondArgument.front != "\"")) {
			if ((LexicalToken::verifyInteger(firstArgument)) || firstArgument.front == "\"") {
				result.insert(PKB().getModifiedVar(firstArgument));
			}
			if (firstArgument == PKB().getProcName()) {
				return strPairSetToStrSet(PKB().getProcVarModifyPairs());
			}
			return intStrSetToStrSet(PKB().getStmVarModifyPairs());
		}
		if ((firstArgument != "_") && (secondArgument.front != "\"")) {
			if (secondArgument == "_") {
				return intSetToStrSet(PKB().getStmModifying(""));
			}
			else if (secondArgument.front == "\"") {
				return intSetToStrSet(PKB().getStmModifying(secondArgument));
			}
			else {
				return result;
			}
		}
		return result;
	}

	return result;
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

unordered_set<string> intPairSetToStrSet(unordered_set<pair<int, int>, intPairhash> intPairSet) {
	unordered_set<string> strSet;
	for (unordered_set<pair<int, int>>::iterator it = intPairSet.begin(); it != intPairSet.end(); ++it) {
		pair<int, int> pointer = *it;
		strSet.insert(to_string(pointer.first) + " " + to_string(pointer.second));
	}

	return strSet;
}

unordered_set<string> intStrSetToStrSet(unordered_set<pair<int, string>, intStringhash> intStringSet) {
	unordered_set<string> strSet;
	for (unordered_set<pair<int, string>>::iterator it = intStringSet.begin(); it != intStringSet.end(); ++it) {
		pair<int, string> pointer = *it;
		strSet.insert(to_string(pointer.first) + " " + pointer.second);
	}

	return strSet;
}

unordered_set<string> strPairSetToStrSet(unordered_set<pair<string, string>, strPairhash> strPairSet) {
	unordered_set<string> strSet;
	for (unordered_set<pair<string, string>>::iterator it = strPairSet.begin(); it != strPairSet.end(); ++it) {
		pair<string, string> pointer = *it;
		strSet.insert(pointer.first + " " + pointer.second);
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
		return PKB().getConstants());
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

unordered_set<string> getFirstParam(unordered_set<string> stringPair) {
	unordered_set<string> result;
	int spaceIndex;
	for (unordered_set<string>::iterator it = stringPair.begin(); it != stringPair.end(); ++it) {
		string pointer = *it;
		spaceIndex = pointer.find(" ");
		result.insert(pointer.substr(0, spaceIndex - 1));
	}

	return result;
}

unordered_set<string> getSecondParam(unordered_set<string> stringPair) {
	unordered_set<string> result;
	int spaceIndex;
	for (unordered_set<string>::iterator it = stringPair.begin(); it != stringPair.end(); ++it) {
		string pointer = *it;
		spaceIndex = pointer.find(" ");
		result.insert(pointer.substr(spaceIndex + 1, pointer.size - spaceIndex - 3));
	}

	return result;
}

unordered_set<string> getOtherPair(int position, unordered_set<string> stringPair, unordered_set<string> toContain) {
	unordered_set<string> result;
	int spaceIndex;
	string first;
	if (position == 1) {
		for (unordered_set<string>::iterator it = stringPair.begin(); it != stringPair.end(); ++it) {
			string pointer = *it;
			spaceIndex = pointer.find(" ");
			first = pointer.substr(0, spaceIndex - 1);
			if (toContain.count(first) == 1) {
				result.insert(pointer.substr(spaceIndex + 1, pointer.size - spaceIndex - 3));
			}
		}
	}
	if (position == 2) {
		for (unordered_set<string>::iterator it = stringPair.begin(); it != stringPair.end(); ++it) {
			string pointer = *it;
			spaceIndex = pointer.find(" ");
			first = pointer.substr(spaceIndex + 1, pointer.size - spaceIndex - 3);
			if (toContain.count(first) == 1) {
				result.insert(pointer.substr(0, spaceIndex - 1));
			}
		}
	}

	return result;
}



