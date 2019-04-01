#pragma once
#include "PKB.h"

class RunTimeDesignExtractor {

public:
	static unordered_set<int> extractNextStar(int stmt);
	static unordered_set<int> extractPreviousStar(int stmt);
	static bool extractNextStarPair(int start, int end);
	static unordered_set<pair<int, int>, intPairhash> getNextStarPairs();
	static void DFSRecursiveNext(int procedure, unordered_set<int> &visitedStmts, unordered_set<int>& stmtsAfter);
	static void DFSRecursivePrevious(int procedure, unordered_set<int> &visitedStmts, unordered_set<int> &stmtsBefore);

private:
	static PKB pkb;
};