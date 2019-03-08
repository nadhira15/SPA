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
#include "ExpressionUtil.h"

/*
The main evaluator function of the query
*/
unordered_set<string> QueryEvaluator::evaluateQuery(vector<pair<string, string>> declarations,
	vector<string> selectedVar, vector<pair<string, pair<string, string>>> suchThatCondition,
	vector<pair<string, pair<string, string>>> patternCondition) {

	return filterSuchThatCondition(declarations, selectedVar, 
		suchThatCondition, patternCondition);
}

/*
It filters the assignment statements
that fulfill the pattern.
*/
unordered_set<string> QueryEvaluator::filterPatternCondition(vector<pair<string, pair<string, string>>> patternCondition) {
	string patternSynonym = patternCondition[0].first;
	string leftArgument = patternCondition[0].second.first;
	string rightArgument = patternCondition[0].second.second;

	if (rightArgument.size() == 1) {
		if (leftArgument == "_") {
			return getStmts("assign");
		} 
		else if (leftArgument[0] == '"') {
			return intVecToStrSet(PKB().findPattern(trimFrontEnd(leftArgument), "", false));
		}
		return intStrVecToStrSet(PKB().findPatternPairs("", false));
	}
	else if (rightArgument[0] == '"') {
		rightArgument = trimFrontEnd(rightArgument);
		rightArgument = ExpressionUtil::convertInfixToPrefix(rightArgument);
		if (leftArgument == "_") {
			return intVecToStrSet(PKB().findPattern(rightArgument, true));
		}
		else if (leftArgument[0] == '"') {
			return intVecToStrSet(PKB().findPattern(trimFrontEnd(leftArgument), rightArgument, true));
		}
		return intStrVecToStrSet(PKB().findPatternPairs(rightArgument, true));
	} 
	else {
		rightArgument = trimFrontEnd(trimFrontEnd(rightArgument));
		rightArgument = ExpressionUtil::convertInfixToPrefix(rightArgument);
		if (leftArgument == "_") {
			return intVecToStrSet(PKB().findPattern(rightArgument, false));
		}
		else if (leftArgument[0] == '"') {
			return intVecToStrSet(PKB().findPattern(trimFrontEnd(leftArgument), rightArgument, false));
		}
		return intStrVecToStrSet(PKB().findPatternPairs(rightArgument, false));
	}
}

/*
The function evaluates the query by evaluating 
its such that condition after given the filter 
result of the Pattern Condition.
*/
unordered_set<string> QueryEvaluator::filterSuchThatCondition(vector<pair<string, string>> declarations, vector<string> selectedVar,
	vector<pair<string, pair<string, string>>> suchThatCondition, vector<pair<string, pair<string, string>>> patternCondition) {
	
	unordered_set<string> emptyResult;
	string selectedVarType;
	unordered_set<string> afterPatternFilter;
	for (vector<pair<string, string>>::size_type i = 0; i != declarations.size(); i++) {
		if (declarations[i].second == selectedVar[0]) {
			selectedVarType = declarations[i].first;
		}
	}
	if (suchThatCondition.size() == 0) {
		if (patternCondition.size() == 0) {
			return getStmts(selectedVarType);
		}
		afterPatternFilter = filterPatternCondition(patternCondition);
		if (afterPatternFilter.size() == 0) {
			return emptyResult;
		}
		if (selectedVar[0] == patternCondition[0].first) {
			if (patternCondition[0].second.first[0] == '"') {
				return afterPatternFilter;
			}
			return getFirstParam(afterPatternFilter);
		}
		return getStmts(selectedVarType);
	}
	string relation = suchThatCondition[0].first;
	string firstArgument = suchThatCondition[0].second.first;
	string secondArgument = suchThatCondition[0].second.second;
	string firstArgumentType;
	string secondArgumentType;
	for (vector<pair<string, string>>::size_type i = 0; i != declarations.size(); i++) {
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
	string certainty = isSuchThatTrivial(relation, firstArgument, secondArgument);

	if (certainty == "false") {
		return emptyResult;
	}
	else if (certainty == "true") {
		if (patternCondition.size() == 0) {
			return getStmts(selectedVarType);
		}
		afterPatternFilter = filterPatternCondition(patternCondition);
		if (afterPatternFilter.size() == 0) {
			return emptyResult;
		}
		if (selectedVar[0] == patternCondition[0].first) {
			if (patternCondition[0].second.first[0] == '"') {
				return afterPatternFilter;
			}
			return getFirstParam(afterPatternFilter);
		}
		return getStmts(selectedVarType);
	}
	else if (certainty == "not trivial") {
		unordered_set<string> suchThatResult = evaluateSuchThat(relation, firstArgument, secondArgument);
		if (suchThatResult.size() == 0) {
			return emptyResult;
		}
		if (patternCondition.size() == 0) {
			if (selectedVar[0] == firstArgument) {
				if (secondArgumentType == "") {
					return filterType(firstArgumentType, suchThatResult);
				} 
				return getFirstParam(filterType(firstArgumentType, 
					secondArgumentType, 
					suchThatResult));
			}
			if (selectedVar[0] == secondArgument) {
				if (firstArgumentType == "") {
					return filterType(secondArgumentType, suchThatResult);
				}
				return getSecondParam(filterType(firstArgumentType,
					secondArgumentType,
					suchThatResult));
			}
			return getStmts(selectedVarType);
		}
		afterPatternFilter = filterPatternCondition(patternCondition);
		string patternSynonym = patternCondition[0].first;
		string patternLeft = patternCondition[0].second.first;
		vector<pair<string, pair<string, string>>> emptyPattern;
		unordered_set<string> fulfillingVar;
		unordered_set<string> fulfillingAssign;
		if (patternLeft[0] == '"' && secondArgument == patternLeft) {
			if (firstArgumentType == "") {
				fulfillingVar = intersection(
					filterType(firstArgumentType, suchThatResult), getSecondParam(afterPatternFilter)
				);
				if (fulfillingVar.size() == 0) {
					return emptyResult;
				}
				if (selectedVar[0] == patternLeft) {
					return fulfillingVar;
				}
				return getStmts(selectedVarType);
			}
			fulfillingVar = intersection(
				filterType(firstArgumentType, secondArgumentType, suchThatResult), 
				getSecondParam(afterPatternFilter)
			);
			if (fulfillingVar.size() == 0) {
				return emptyResult;
			}
			if (selectedVar[0] == patternLeft) {
				return fulfillingVar;
			}
			if (selectedVar[0] == firstArgument) {
				if (firstArgument == patternSynonym) {
					return getFirstParam(
						intersection(
							filterType(firstArgumentType, secondArgumentType, suchThatResult),
							getSecondParam(afterPatternFilter)
						)
					);
				}
				return getOtherPair(2, filterType(firstArgumentType, secondArgumentType, suchThatResult),
					fulfillingVar);
			}
			if (selectedVar[0] == patternSynonym) {
				return getOtherPair(2, afterPatternFilter, fulfillingVar);
			}
			if (firstArgument == patternSynonym) {
				fulfillingAssign = getFirstParam(
					intersection(
						filterType(firstArgumentType, secondArgumentType, suchThatResult),
						getSecondParam(afterPatternFilter)
					));
				if (fulfillingAssign.size() == 0) {
					return emptyResult;
				}
				return getStmts(selectedVarType);
			}
			return getStmts(selectedVarType);
		}
		
		if (selectedVar[0] != patternSynonym && 
			firstArgument != patternSynonym &&
			secondArgument != patternSynonym) {
			if (afterPatternFilter.size() == 0) {
				return emptyResult;
			}
			return filterSuchThatCondition(declarations, selectedVar, suchThatCondition, emptyPattern);
		}
		if ((firstArgument != patternSynonym) && (secondArgument != patternSynonym)) {
			return getAssign(afterPatternFilter);
		}
		if (firstArgument == patternSynonym) {
			if (selectedVar[0] == patternSynonym) {
				if (secondArgumentType == "") {
					return intersection(
						filterType(firstArgumentType, suchThatResult), getAssign(afterPatternFilter));
				}
				return intersection(
					getFirstParam(
						filterType(firstArgumentType, secondArgumentType, suchThatResult)), 
					getAssign(afterPatternFilter));
			}
			return getOtherPair(1, filterType(
				firstArgumentType, secondArgumentType, suchThatResult), 
				getAssign(afterPatternFilter));
		}
		if (secondArgument == patternSynonym) {
			if (selectedVar[0] == patternSynonym) {
				if (firstArgumentType == "") {
					return intersection(filterType(
						secondArgumentType, suchThatResult), getAssign(afterPatternFilter));
				}
				return intersection(
					getSecondParam(
						filterType(firstArgumentType, secondArgumentType, suchThatResult)), 
					getAssign(afterPatternFilter));
			}
			return getOtherPair(2, filterType(
				firstArgumentType, secondArgumentType, suchThatResult), 
				getAssign(afterPatternFilter));
		}
	}

	return emptyResult;
}

/*
THe function evaluates the such that conditions 
which gives boolean answer
*/
string QueryEvaluator::isSuchThatTrivial(string relation, string firstArgument, string secondArgument) {
	if (relation == "Follows") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasFollowRelation());
			}
			else if (LexicalToken::verifyInteger(secondArgument)) {
				return truthValue(PKB().getPrvStm(stoi(secondArgument)) > 0);
			}
			else if (firstArgument == secondArgument) {
				return "false";
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
			else if (firstArgument == secondArgument) {
				return "false";
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
			else if (firstArgument == secondArgument) {
				return "false";
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
			else if (firstArgument == secondArgument) {
				return "false";
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
			if (secondArgument.find("\"") != string::npos) {
				return truthValue(PKB().isUsing(stoi(firstArgument), secondArgument));
			}
			else if (secondArgument == "_") {
				return truthValue(PKB().getUsedVar(stoi(firstArgument)).size() > 0);
			}
			return "not trivial";
		}
		else if (firstArgument.find("\"") != string::npos) {
			if (secondArgument.find("\"") != string::npos) {
				return truthValue(PKB().isUsing(firstArgument, secondArgument));
			}
			else if (secondArgument == "_") {
				return truthValue(PKB().getUsedVar(firstArgument).size() > 0);
			}
			return "not trivial";
		}
		return "not trivial";
	}
	else if (relation == "Modifies") {
		if (LexicalToken::verifyInteger(firstArgument)) {
			if (secondArgument.find("\"") != string::npos) {
				return truthValue(PKB().isModifying(stoi(firstArgument), secondArgument));
			}
			else if (secondArgument == "_") {
				return truthValue(PKB().getModifiedVar(stoi(firstArgument)).size() > 0);
			}
			return "not trivial";
		}
		else if (firstArgument.find("\"") != string::npos) {
			if (secondArgument.find("\"") != string::npos) {
				return truthValue(PKB().isModifying(firstArgument, secondArgument));
			}
			else if (secondArgument == "_") {
				return truthValue(PKB().getModifiedVar(firstArgument).size() > 0);
			}
			return "not trivial";
		}
		return "not trivial";
	}

	return "not trivial";
}

/*
The function evaluates the non-trivial 
Such That Conditions.
*/
unordered_set<string> QueryEvaluator::evaluateSuchThat(string relation, string firstArgument, string secondArgument) {
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
				result.insert(to_string(PKB().getNxtStm(stoi(firstArgument))));
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
				return intSetToStrSet(PKB().getAllFollowing(stoi(firstArgument)));
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
				return intSetToStrSet(PKB().getChildren(stoi(firstArgument)));
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
				return intSetToStrSet(PKB().getAllDescendants(stoi(firstArgument)));
			}
			else {
				return result;
			}
		}
		return result;
	}
	else if (relation == "Uses") {
		if ((secondArgument != "_") && (secondArgument.find("\"") == string::npos)) {
			if ((LexicalToken::verifyInteger(firstArgument)) || firstArgument.find("\"") != string::npos) {
				return PKB().getUsedVar(firstArgument);
			} 
			if (firstArgument == PKB().getProcName()) {
				return strPairSetToStrSet(PKB().getProcVarUsePairs());
			}
			return intStrSetToStrSet(PKB().getStmVarUsePairs());
		}
		if ((firstArgument != "_") && (firstArgument.find("\"")) == string::npos) {
			if (secondArgument == "_") {
				return intSetToStrSet(PKB().getStmUsing(""));
			}
			else if (secondArgument.find("\"") != string::npos) {
				return intSetToStrSet(PKB().getStmUsing(secondArgument));
			} 
			else {
				return result;
			}
		}
		return result;
	}
	else if (relation == "Modifies") {
		if ((secondArgument != "_") && (secondArgument.find("\"") == string::npos)) {
			if ((LexicalToken::verifyInteger(firstArgument)) || firstArgument.find("\"") != string::npos) {
				result = PKB().getModifiedVar(firstArgument);
			}
			if (firstArgument == PKB().getProcName()) {
				return strPairSetToStrSet(PKB().getProcVarModifyPairs());
			}
			return intStrSetToStrSet(PKB().getStmVarModifyPairs());
		}
		if ((firstArgument != "_") && (firstArgument.find("\"") == string::npos)) {
			if (secondArgument == "_") {
				return intSetToStrSet(PKB().getStmModifying(""));
			}
			else if (secondArgument.find("\"") != string::npos) {
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

/*
The function returns the list of all statements.
*/
unordered_set<string> QueryEvaluator::getAllStms() {
	unordered_set<string> allStms;
	for (int i = 1; i <= PKB().getTotalStmNo(); i++) {
		allStms.insert(to_string(i));
	}

	return allStms;
}

/*
The function transforms a vector of integers
into a set of strings
*/
unordered_set<string> QueryEvaluator::intVecToStrSet(vector<int> intVec) {
	unordered_set<string> strSet;
	for (vector<int>::size_type i = 0; i != intVec.size(); i++) {
		strSet.insert(to_string(intVec[i]));
	}

	return strSet;
}

/*
The function transforms a vector of pair of integer and string
into a set of strings
*/
unordered_set<string> QueryEvaluator::intStrVecToStrSet(vector<pair<int, string>> intStrVec) {
	unordered_set<string> strSet;
	for (vector<pair<int, string>>::size_type i = 0; i != intStrVec.size(); i++) {
		strSet.insert(to_string(intStrVec[i].first) + " " + intStrVec[i].second);
	}

	return strSet;
}

/*
The function transforms a set of integers
into a set of strings
*/
unordered_set<string> QueryEvaluator::intSetToStrSet(unordered_set<int> intSet) {
	unordered_set<string> strSet;
	for (unordered_set<int>::iterator it = intSet.begin(); it != intSet.end(); ++it) {
		strSet.insert(to_string(*it));
	}

	return strSet;
}

/*
The function transforms a boolean value
into a string.
*/
string QueryEvaluator::truthValue(bool boolean) {
	if (boolean) {
		return "true";
	}

	return "false";
}

/*
It transforms a set of pairs of integers into
a set of pairs of strings.
*/
unordered_set<string> QueryEvaluator::intPairSetToStrSet(unordered_set<pair<int, int>, intPairhash> intPairSet) {
	unordered_set<string> strSet;
	for (unordered_set<pair<int, int>, intPairhash>::iterator it = intPairSet.begin(); it != intPairSet.end(); ++it) {
		pair<int, int> pointer = *it;
		strSet.insert(to_string(pointer.first) + " " + to_string(pointer.second));
	}

	return strSet;
}

/*
It transforms a set of pair of integer
and string into a set of strings consist
of an integer and a string
*/
unordered_set<string> QueryEvaluator::intStrSetToStrSet(unordered_set<pair<int, string>, intStringhash> intStringSet) {
	unordered_set<string> strSet;
	for (unordered_set<pair<int, string>, intStringhash>::iterator it = intStringSet.begin(); it != intStringSet.end(); ++it) {
		pair<int, string> pointer = *it;
		strSet.insert(to_string(pointer.first) + " " + pointer.second);
	}

	return strSet;
}

/*
It transforms a set of pair of strings
into a set of strings of pairs.
*/
unordered_set<string> QueryEvaluator::strPairSetToStrSet(unordered_set<pair<string, string>, strPairhash> strPairSet) {
	unordered_set<string> strSet;
	for (unordered_set<pair<string, string>, strPairhash>::iterator it = strPairSet.begin(); it != strPairSet.end(); ++it) {
		pair<string, string> pointer = *it;
		strSet.insert(pointer.first + " " + pointer.second);
	}

	return strSet;
}

/*
The function retrieves all statements 
of a given type
*/
unordered_set<string> QueryEvaluator::getStmts(string s) {
	unordered_set<string> result;
	if (s == "stmt") {
		return getAllStms();
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
		return PKB().getConstants();
	}
	else if (s == "procedure") {
		result.insert(PKB().getProcName());
	}
	return result;
}

/*
The function filters a set of
strings so that they are in the
set of all statements of a given
type.
*/
unordered_set<string> QueryEvaluator::filterType(string typeRequired, unordered_set<string>toBeFiltered) {
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

/*
The function filters the set of strings
of pairs so that the first member and 
second member is in the set of all 
statements in a first type and a second
type respectively.
*/
unordered_set<string> QueryEvaluator::filterType(string firstTypeRequired, string secondTypeRequired,
	unordered_set<string> toBeFiltered) {
	unordered_set<string> firstTypeRequiredSet = getStmts(firstTypeRequired);
	unordered_set<string> secondTypeRequiredSet = getStmts(secondTypeRequired);
	unordered_set<string> filteredSet;
	for (unordered_set<string>::iterator it = toBeFiltered.begin(); it != toBeFiltered.end(); ++it) {
		string pointer = *it;
		int spaceIndex = pointer.find(" ");
		string firstParam = pointer.substr(0, spaceIndex);
		string secondParam = pointer.substr(spaceIndex + 1, pointer.size() - spaceIndex - 3);
		if ((firstTypeRequiredSet.count(firstParam) == 1) &&
			(secondTypeRequiredSet.count(secondParam) == 1)) {
			filteredSet.insert(pointer);
		}
	}

	return filteredSet;
}

/*
It fetchs out all the first member of
all pairs in the set
*/
unordered_set<string> QueryEvaluator::getFirstParam(unordered_set<string> stringPair) {
	unordered_set<string> result;
	int spaceIndex;
	for (unordered_set<string>::iterator it = stringPair.begin(); it != stringPair.end(); ++it) {
		string pointer = *it;
		spaceIndex = pointer.find(" ");
		result.insert(pointer.substr(0, spaceIndex));
	}

	return result;
}

/*
It fetchs out all the second member
of all pairs in the set.
*/
unordered_set<string> QueryEvaluator::getSecondParam(unordered_set<string> stringPair) {
	unordered_set<string> result;
	int spaceIndex;
	for (unordered_set<string>::iterator it = stringPair.begin(); it != stringPair.end(); ++it) {
		string pointer = *it;
		spaceIndex = pointer.find(" ");
		result.insert(pointer.substr(spaceIndex + 1, pointer.size() - spaceIndex - 3));
	}

	return result;
}

/*
Finds the intersection of 2 sets of string
*/
unordered_set<string> QueryEvaluator::intersection(unordered_set<string> first, unordered_set<string> toContain) {
	unordered_set<string> result;
	for (unordered_set<string>::iterator it = first.begin(); it != first.end(); ++it) {
		string pointer = *it;
		if (toContain.count(pointer) == 1) {
			result.insert(pointer);
		}
	}

	return result;
}

/*
The function does the following:
1. It checks which pair in stringPair which 
member in position "position"(either 1 or 2)
is in the toContain set.
2. It fetch out all the member in the 
other "position" of the filtered set of pairs.
*/
unordered_set<string> QueryEvaluator::getOtherPair(int position, unordered_set<string> stringPair, unordered_set<string> toContain) {
	unordered_set<string> result;
	int spaceIndex;
	string first;
	if (position == 1) {
		for (unordered_set<string>::iterator it = stringPair.begin(); it != stringPair.end(); ++it) {
			string pointer = *it;
			spaceIndex = pointer.find(" ");
			first = pointer.substr(0, spaceIndex);
			if (toContain.count(first) == 1) {
				result.insert(pointer.substr(spaceIndex + 1, pointer.size() - spaceIndex - 3));
			}
		}
	}
	if (position == 2) {
		for (unordered_set<string>::iterator it = stringPair.begin(); it != stringPair.end(); ++it) {
			string pointer = *it;
			spaceIndex = pointer.find(" ");
			first = pointer.substr(spaceIndex + 1, pointer.size() - spaceIndex - 3);
			if (toContain.count(first) == 1) {
				result.insert(pointer.substr(0, spaceIndex));
			}
		}
	}

	return result;
}

/*
Retrieves the assign statements
arguments after pattern filtering
*/
unordered_set<string> QueryEvaluator::getAssign(unordered_set<string> afterPatternFilter) {
	unordered_set<string> result;
	for (unordered_set<string>::iterator it = afterPatternFilter.begin(); it != afterPatternFilter.end(); ++it) {
		string pointer = *it;
		if (pointer.find(" ") != string::npos) {
			result.insert(pointer.substr(0, pointer.find(" ")));
		}
		else {
			result.insert(pointer);
		}
	}

	return result;
}


/*
Trims quote in front and end of a string.
*/
string QueryEvaluator::trimFrontEnd(string quotedString) {
	return quotedString.substr(1, quotedString.size() - 2);
}

