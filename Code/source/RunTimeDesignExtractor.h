#pragma once
#include "PKB.h"

class RunTimeDesignExtractor {

public:
	static unordered_set<int> extractNextStar(int stmt);
	static unordered_set<int> extractPreviousStar(int stmt);
	static bool extractNextStarPair(int start, int end);
	static void DFSRecursiveNext(int procedure, unordered_set<int> &visitedStmts);
	static void DFSRecursivePrevious(int procedure, unordered_set<int> &visitedStmts);

private:
	static PKB pkb;
};