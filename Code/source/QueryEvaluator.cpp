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
#include "ContainerUtil.h"

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
		else if (isQuoted(leftArgument)) {
			return ContainerUtil::to_strset(PKB().findPattern(trimFrontEnd(leftArgument), "", false));
		}
		return ContainerUtil::to_strset(PKB().findPatternPairs("", false));
	}
	else if (isQuoted(rightArgument)) {
		rightArgument = trimFrontEnd(rightArgument);
		rightArgument = ExpressionUtil::convertInfixToPrefix(rightArgument);
		if (leftArgument == "_") {
			return ContainerUtil::to_strset(PKB().findPattern(rightArgument, true));
		}
		else if (isQuoted(leftArgument)) {
			return ContainerUtil::to_strset(PKB().findPattern(trimFrontEnd(leftArgument), rightArgument, true));
		}
		return ContainerUtil::to_strset(PKB().findPatternPairs(rightArgument, true));
	} 
	else {
		rightArgument = trimFrontEnd(trimFrontEnd(rightArgument));
		rightArgument = ExpressionUtil::convertInfixToPrefix(rightArgument);
		if (leftArgument == "_") {
			return ContainerUtil::to_strset(PKB().findPattern(rightArgument, false));
		}
		else if (isQuoted(leftArgument)) {
			return ContainerUtil::to_strset(PKB().findPattern(trimFrontEnd(leftArgument), rightArgument, false));
		}
		return ContainerUtil::to_strset(PKB().findPatternPairs(rightArgument, false));
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
			if (isQuoted(patternCondition[0].second.first)) {
				return afterPatternFilter;
			}
			return ContainerUtil::getFirstParam(afterPatternFilter);
		}
		if (selectedVar[0] == patternCondition[0].second.first) {
			return ContainerUtil::getSecondParam(afterPatternFilter);
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
			if (isQuoted(patternCondition[0].second.first)) {
				return afterPatternFilter;
			}
			return ContainerUtil::getFirstParam(afterPatternFilter);
		}
		return getStmts(selectedVarType);
	}
	else if (certainty == "not trivial") {
		unordered_set<string> suchThatResult = evaluateSuchThat(relation, firstArgument, secondArgument);
		if (firstArgumentType != "" && secondArgumentType == "") {
			suchThatResult = filterType(firstArgumentType, suchThatResult);
		}
		else if (firstArgumentType == "" && secondArgumentType != "") {
			suchThatResult = filterType(secondArgumentType, suchThatResult);
		} 
		else if (firstArgumentType != "" && secondArgumentType != "") {
			suchThatResult = filterType(firstArgumentType, secondArgumentType, suchThatResult);
		}
		if (suchThatResult.size() == 0) {
			return emptyResult;
		}
		if (patternCondition.size() == 0) {
			if (selectedVar[0] == firstArgument) {
				if (secondArgumentType == "") {
					return suchThatResult;
				} 
				return ContainerUtil::getFirstParam(suchThatResult);
			}
			if (selectedVar[0] == secondArgument) {
				if (firstArgumentType == "") {
					return suchThatResult;
				}
				return ContainerUtil::getSecondParam(suchThatResult);
			}
			return getStmts(selectedVarType);
		}
		afterPatternFilter = filterPatternCondition(patternCondition);
		string patternSynonym = patternCondition[0].first;
		string patternLeft = patternCondition[0].second.first;
		vector<pair<string, pair<string, string>>> emptyPattern;
		if (!isQuoted(patternLeft) && secondArgument == patternLeft) {
			unordered_set<string> fulfillingVar;
			if (firstArgumentType == "") {
				fulfillingVar = ContainerUtil::intersection(
					suchThatResult, 
					ContainerUtil::getSecondParam(afterPatternFilter)
				);
				if (fulfillingVar.size() == 0) {
					return emptyResult;
				}
				if (selectedVar[0] == patternLeft) {
					return fulfillingVar;
				}
				return getStmts(selectedVarType);
			}
			if (firstArgument == patternSynonym) {
				unordered_set<string> fulfillingPair = ContainerUtil::intersection(
					suchThatResult, afterPatternFilter
				);
				if (fulfillingPair.size() == 0) {
					return emptyResult;
				}
				if (selectedVar[0] == firstArgument) {
					return ContainerUtil::getFirstParam(fulfillingPair);
				}
				if (selectedVar[0] == secondArgument) {
					return ContainerUtil::getSecondParam(fulfillingPair);
				}
				return getStmts(selectedVarType);
			}
			fulfillingVar = ContainerUtil::intersection(
				ContainerUtil::getSecondParam(suchThatResult), 
				ContainerUtil::getSecondParam(afterPatternFilter)
			);
			if (fulfillingVar.size() == 0) {
				return emptyResult;
			}
			if (selectedVar[0] == patternLeft) {
				return fulfillingVar;
			}
			if (selectedVar[0] == firstArgument) {
				return ContainerUtil::getOtherPair(2, suchThatResult, fulfillingVar);
			}
			if (selectedVar[0] == patternSynonym) {
				return ContainerUtil::getOtherPair(2, afterPatternFilter, fulfillingVar);
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
					return ContainerUtil::intersection(suchThatResult, getAssign(afterPatternFilter));
				}
				return ContainerUtil::intersection(
					ContainerUtil::getFirstParam(suchThatResult), 
					getAssign(afterPatternFilter));
			}
			return ContainerUtil::getOtherPair(1, suchThatResult, getAssign(afterPatternFilter));
		}
		if (secondArgument == patternSynonym) {
			if (selectedVar[0] == patternSynonym) {
				if (firstArgumentType == "") {
					return ContainerUtil::intersection(suchThatResult, getAssign(afterPatternFilter));
				}
				return ContainerUtil::intersection(
					ContainerUtil::getSecondParam(suchThatResult), 
					getAssign(afterPatternFilter));
			}
			return ContainerUtil::getOtherPair(2, suchThatResult, getAssign(afterPatternFilter));
		}
	}

	return emptyResult;
}

/*
THe function evaluates the such that conditions 
which gives boolean answer
*/
string QueryEvaluator::isSuchThatTrivial(string relation, string firstArgument, string secondArgument) {
	bool result;
	if (relation == "Follows") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasFollowRelation());
			}
			else if (isInteger(secondArgument)) {
				result = PKB().getStmFollowedBy(stoi(secondArgument)) > 0;
				return truthValue(result);
			}
		}
		else if (isInteger(firstArgument)) {
			if (secondArgument == "_") {
				result = PKB().getFollower(stoi(firstArgument)) > 0;
				return truthValue(result);
 			}
			else if (isInteger(secondArgument)) {
				result = PKB().getFollower(stoi(firstArgument)) == stoi(secondArgument);
				return truthValue(result);
			};
		}
		else if (firstArgument == secondArgument) {
			return "false";
		}
	}
	else if (relation == "Follows*") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasFollowRelation());
			}
			else if (isInteger(secondArgument)) {
				result = PKB().getStmFollowedBy(stoi(secondArgument)) > 0;
				return truthValue(result);
			}
		}
		else if (isInteger(firstArgument)) {
			if (secondArgument == "_") {
				result = PKB().getFollower(stoi(firstArgument)) > 0;
				return truthValue(result);
			}
			else if (isInteger(secondArgument)) {
				result = PKB().hasFollowStarPair(stoi(firstArgument), stoi(secondArgument));
				return truthValue(result);
			}
		}
		else if (firstArgument == secondArgument) {
			return "false";
		}
	}
	else if (relation == "Parent") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasParentRelation());
			}
			else if (isInteger(secondArgument)) {
				result = PKB().isChild(stoi(secondArgument)) > 0;
				return truthValue(result);
			}
		}
		else if (isInteger(firstArgument)) {
			if (secondArgument == "_") {
				result = PKB().isParent(stoi(firstArgument)) > 0;
				return truthValue(result);
			}
			else if (isInteger(secondArgument)) {
				result = PKB().getParent(stoi(secondArgument)) == stoi(firstArgument);
				return truthValue(result);
			}
		}
		else if (firstArgument == secondArgument) {
			return "false";
		}
	}
	else if (relation == "Parent*") {
		if (firstArgument == "_") {
			if (secondArgument == "_") {
				return truthValue(PKB().hasParentRelation());
			}
			else if (isInteger(secondArgument)) {
				result = PKB().isChild(stoi(secondArgument)) > 0;
				return truthValue(result);
			}
		}
		else if (isInteger(firstArgument)) {
			if (secondArgument == "_") {
				result = PKB().isParent(stoi(firstArgument)) > 0;
				return truthValue(result);
			}
			else if (isInteger(secondArgument)) {
				result = PKB().hasAncDescPair(stoi(firstArgument), stoi(secondArgument));
				return truthValue(result);
			}
		}
		else if (firstArgument == secondArgument) {
			return "false";
		}
	}
	else if (relation == "Uses") {
		if (isInteger(firstArgument)) {
			if (isQuoted(secondArgument)) {
				result = PKB().isUsing(stoi(firstArgument), trimFrontEnd(secondArgument));
				return truthValue(result);
			}
			else if (secondArgument == "_") {
				result = PKB().getVarUsedByStm(stoi(firstArgument)).size() > 0;
				return truthValue(result);
			}
		}
		else if (isQuoted(firstArgument)) {
			if (isQuoted(secondArgument)) {
				result = PKB().isUsing(trimFrontEnd(firstArgument), trimFrontEnd(secondArgument));
				return truthValue(result);
			}
			else if (secondArgument == "_") {
				result = PKB().getVarUsedByProc(trimFrontEnd(firstArgument)).size() > 0;
				return truthValue(result);
			}
		}
	}
	else if (relation == "Modifies") {
		if (isInteger(firstArgument)) {
			if (isQuoted(secondArgument)) {
				result = PKB().isModifying(stoi(firstArgument), trimFrontEnd(secondArgument));
				return truthValue(result);
			}
			else if (secondArgument == "_") {
				result = PKB().getModifiedVar(stoi(firstArgument)).size() > 0;
				return truthValue(result);
			}
		}
		else if (isQuoted(firstArgument)) {
			if (isQuoted(secondArgument)) {
				result = PKB().isModifying(trimFrontEnd(firstArgument), trimFrontEnd(secondArgument));
				return truthValue(result);
			}
			else if (secondArgument == "_") {
				result = PKB().getModifiedVar(trimFrontEnd(firstArgument)).size() > 0;
				return truthValue(result);
			}
		}
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
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				return ContainerUtil::to_strset(PKB().getAllFollowed());
			}
			else if (isInteger(secondArgument)) {
				result.insert(to_string(PKB().getStmFollowedBy(stoi(secondArgument))));
			}
			else {
				return ContainerUtil::to_strset(PKB().getFollowPairs());
			}
		}
		if (isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				return ContainerUtil::to_strset(PKB().getAllFollowers());
			}
			else if (isInteger(firstArgument)) {
				result.insert(to_string(PKB().getFollower(stoi(firstArgument))));
			}
			else {
				return result;
			}
		}
		return result;
	}
	else if (relation == "Follows*") {
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				return ContainerUtil::to_strset(PKB().getAllFollowed());
			}
			else if (isInteger(secondArgument)) {
				return ContainerUtil::to_strset(PKB().getAllFollowedBy(stoi(secondArgument)));
			}
			else {
				return ContainerUtil::to_strset(PKB().getFollowStarPairs());
			}
		}
		if (isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				return ContainerUtil::to_strset(PKB().getAllFollowers());
			}
			else if (isInteger(firstArgument)) {
				return ContainerUtil::to_strset(PKB().getAllFollowing(stoi(firstArgument)));
			}
			else {
				return result;
			}
		}
		return result;
	}
	if (relation == "Parent") {
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				return ContainerUtil::to_strset(PKB().getAllParents());
			}
			else if (isInteger(secondArgument)) {
				result.insert(to_string(PKB().getParent(stoi(secondArgument))));
			}
			else {
				return ContainerUtil::to_strset(PKB().getParentChildPairs());
			}
		}
		if (isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				return ContainerUtil::to_strset(PKB().getAllChildren());
			}
			else if (isInteger(firstArgument)) {
				return ContainerUtil::to_strset(PKB().getChildren(stoi(firstArgument)));
			}
			else {
				return result;
			}
		}
		return result;
	}
	else if (relation == "Parent*") {
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				return ContainerUtil::to_strset(PKB().getAllParents());
			}
			else if (isInteger(secondArgument)) {
				return ContainerUtil::to_strset(PKB().getAncestors(stoi(secondArgument)));
			}
			else {
				return ContainerUtil::to_strset(PKB().getAncDescPairs());
			}
		}
		if (isSynonym(secondArgument)) {
			if (firstArgument == "_") {
				return ContainerUtil::to_strset(PKB().getAllChildren());
			}
			else if (isInteger(firstArgument)) {
				return ContainerUtil::to_strset(PKB().getDescendants(stoi(firstArgument)));
			}
			else {
				return result;
			}
		}
		return result;
	}
	else if (relation == "Uses") {
		if (isSynonym(secondArgument)) {
			if (isInteger(firstArgument)) {
				return PKB().getVarUsedByStm(stoi(firstArgument));
			} 
			else if (isQuoted(firstArgument)) {
				return PKB().getVarUsedByProc(trimFrontEnd(firstArgument));
			}
			if (firstArgument == PKB().getProcName()) {
				return ContainerUtil::to_strset(PKB().getProcVarUsePairs());
			}
			return ContainerUtil::to_strset(PKB().getStmVarUsePairs());
		}
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				return ContainerUtil::to_strset(PKB().getStmUsing(""));
			}
			else if (isQuoted(secondArgument)) {
				return ContainerUtil::to_strset(PKB().getStmUsing(trimFrontEnd(secondArgument)));
			} 
			else {
				return result;
			}
		}
		return result;
	}
	else if (relation == "Modifies") {
		if (isSynonym(secondArgument)) {
			if (isInteger(firstArgument)) {
				return PKB().getModifiedVar(stoi(firstArgument));

			}
			else if (isQuoted(firstArgument)) {
				return PKB().getModifiedVar(trimFrontEnd(firstArgument));
			}
			if (firstArgument == PKB().getProcName()) {
				return ContainerUtil::to_strset(PKB().getProcVarModifyPairs());
			}
			return ContainerUtil::to_strset(PKB().getStmVarModifyPairs());
		}
		if (isSynonym(firstArgument)) {
			if (secondArgument == "_") {
				return ContainerUtil::to_strset(PKB().getStmModifying(""));
			}
			else if (isQuoted(secondArgument)) {
				return ContainerUtil::to_strset(PKB().getStmModifying(trimFrontEnd(secondArgument)));
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
The function retrieves all statements 
of a given type
*/
unordered_set<string> QueryEvaluator::getStmts(string s) {
	unordered_set<string> result;
	if (s == "stmt") {
		return getAllStms();
	}
	else if (s == "read") {
		return ContainerUtil::to_strset(PKB().getReadStms());
	}
	else if (s == "print") {
		return ContainerUtil::to_strset(PKB().getPrintStms());
	}
	else if (s == "while") {
		return ContainerUtil::to_strset(PKB().getWhileStms());
	}
	else if (s == "if") {
		return ContainerUtil::to_strset(PKB().getIfStms());
	}
	else if (s == "assign") {
		return ContainerUtil::to_strset(PKB().getAssignStms());
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
		string secondParam = pointer.substr(spaceIndex + 1, pointer.size() - spaceIndex - 1);
		if ((firstTypeRequiredSet.count(firstParam) == 1) &&
			(secondTypeRequiredSet.count(secondParam) == 1)) {
			filteredSet.insert(pointer);
		}
	}

	return filteredSet;
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
Trims quote in front and end of a string.
*/
string QueryEvaluator::trimFrontEnd(string quotedString) {
	return quotedString.substr(1, quotedString.size() - 2);
}

/*
Checks if the string is an integer
*/
bool QueryEvaluator::isInteger(string s) {
	bool result = LexicalToken::verifyInteger(s);
	return result;
}

/*
Checks if the string is an integer
*/
bool QueryEvaluator::isQuoted(string s) {
	bool result = s[0] == '"';
	return result;
}

/*
Checks if the string is an integer
*/
bool QueryEvaluator::isSynonym(string s) {
	bool result = !isInteger(s) && !isQuoted(s) && s != "_";
	return result;
}
