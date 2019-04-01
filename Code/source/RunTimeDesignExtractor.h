#pragma once
#include "Hasher.h"
#include "PKB.h"

class PKB;

class RunTimeDesignExtractor {

public:
	static unordered_set<int> extractNextStar(PKB* pkb, int stmt);
	static unordered_set<int> extractPreviousStar(PKB* pkb, int stmt);
	static bool extractNextStarPair(PKB* pkb, int start, int end);
	static unordered_set<pair<int, int>, intPairhash> getNextStarPairs(PKB* pkb);
	static void DFSRecursiveNext(PKB* pkb, int procedure, unordered_set<int> &visitedStmts, unordered_set<int>& stmtsAfter);
	static void DFSRecursivePrevious(PKB* pkb, int procedure, unordered_set<int> &visitedStmts, unordered_set<int> &stmtsBefore);

};