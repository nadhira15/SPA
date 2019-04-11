#pragma once

#include "AffectsStarEvaluator.h"	

/*
Checks if AffectsStar clause is trivial
*/
std::string AffectsStarEvaluator::isTrivial(std::string firstArgument, std::string secondArgument) {
	if (firstArgument == "_") {
		if (secondArgument == "_") {
			return QueryUtility::truthValue(PKB().hasAffectsRelation());
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			return QueryUtility::truthValue(PKB().isAffected(stoi(secondArgument)));
		}
	}
	else if (QueryUtility::isInteger(firstArgument)) {
		if (secondArgument == "_") {
			return QueryUtility::truthValue(PKB().isAffector(stoi(firstArgument)));
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			return QueryUtility::truthValue(PKB().hasAffectStarPair(stoi(firstArgument), stoi(secondArgument)));
		}
	}
	return "NON TRIVIAL";
}

/*
Evaluate non trivial AffectsStar clause
*/
std::unordered_map<std::string, std::vector<std::string>> AffectsStarEvaluator::evaluate(
	std::unordered_map<std::string, std::string> declarations,
	std::string firstArgument, std::string secondArgument) {
	std::unordered_map<std::string, std::vector<std::string>> tableResult;
	std::vector<std::string> emptyVec;
	if (QueryUtility::isSynonym(firstArgument)) {
		if (secondArgument == "_") {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAllAffectors());
		}
		else if (QueryUtility::isInteger(secondArgument)) {
			tableResult = QueryUtility::to_mapvec(firstArgument, PKB().getAffectorStar(
				stoi(secondArgument)));
		}
		else if (firstArgument == secondArgument) {
			tableResult = QueryUtility::to_mapvec(firstArgument, QueryUtility::getTwin(
				PKB().getAffectStarPairs()));
		}
		else {
			tableResult = QueryUtility::to_mapvec(firstArgument, secondArgument,
				PKB().getAffectStarPairs());
		}
	}
	if (QueryUtility::isSynonym(secondArgument)) {
		if (firstArgument == "_") {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getAllAffected());
		}
		else if (QueryUtility::isInteger(firstArgument)) {
			tableResult = QueryUtility::to_mapvec(secondArgument, PKB().getAffectedStar(
				stoi(firstArgument)));
		}
	}
	return tableResult;
}