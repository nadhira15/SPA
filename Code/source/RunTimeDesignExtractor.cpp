#include"RunTimeDesignExtractor.h"
#include"Hasher.h"
#include"PKB.h"

PKB RunTimeDesignExtractor::pkb;

unordered_set<int> RunTimeDesignExtractor::extractNextStar(int stmt) {
	unordered_set<int>visitedStmts;
	unordered_set<int>stmtsAfter;
	DFSRecursiveNext(stmt, visitedStmts, stmtsAfter);
	return stmtsAfter;
}

unordered_set<int> RunTimeDesignExtractor::extractPreviousStar(int stmt) {
	unordered_set<int>visitedStmts;
	unordered_set<int>stmtsBefore;
	DFSRecursivePrevious(stmt, visitedStmts,stmtsBefore);
	return stmtsBefore;
}

bool RunTimeDesignExtractor::extractNextStarPair(int start, int end) {
	unordered_set<int>visitedStmts;
	unordered_set<int>stmtsAfter;
	DFSRecursiveNext(start, visitedStmts,stmtsAfter);
	if (stmtsAfter.find(end) == stmtsAfter.end()) {
		return false;
	}
	return true;
}

unordered_set<pair<int, int>, intPairhash> RunTimeDesignExtractor::getNextStarPairs() {
	int stmtNum = pkb.getTotalStmNo();
	unordered_set<pair<int, int>, intPairhash> allNextStarPairs;
	//for all Statement in statemnet list generate nextStar
	for (int i = 1; i <= stmtNum; i++) {
		unordered_set<int>visitedStmts;
		unordered_set<int>stmtsAfter;
		DFSRecursiveNext(i, visitedStmts,stmtsAfter);
		for (int statments : stmtsAfter) {
			allNextStarPairs.emplace(i, statments);
		}
	}
	return allNextStarPairs;
}

void RunTimeDesignExtractor::DFSRecursiveNext(int statments, unordered_set<int> &visitedStmts, unordered_set<int> &stmtsAfter) {
	//Marks statment as visited
	visitedStmts.insert(statments);
	//Get neighbouring statments.
	unordered_set<int> adjacentStmts = pkb.getNext(statments);

	//For each neighbouring statments
	for (int stmts : adjacentStmts) {
		
		std::unordered_set<int>::const_iterator visited = stmtsAfter.find(stmts);
		if (visited == stmtsAfter.end()) {
			stmtsAfter.insert(stmts);
		}
		//Check if neighbouring statments has been visited
		std::unordered_set<int>::const_iterator exist = visitedStmts.find(stmts);

		//If neighbouring statments has not been visited, visit it.
		if (exist == visitedStmts.end()) {
			stmtsAfter.insert(stmts);
			DFSRecursiveNext(stmts, visitedStmts,stmtsAfter);
		}
	}
}

void RunTimeDesignExtractor::DFSRecursivePrevious(int statments, unordered_set<int> &visitedStmts, unordered_set<int> &stmtsBefore) {
	//Marks statment as visited
	visitedStmts.insert(statments);
	//Get neighbouring statments.
	unordered_set<int> adjacentStmts = pkb.getPrev(statments);

	//For each neighbouring statments
	for (int stmts : adjacentStmts) {
		std::unordered_set<int>::const_iterator visited = stmtsBefore.find(stmts);

		if (visited == stmtsBefore.end()) {
			stmtsBefore.insert(stmts);
		}
		//Check if neighbouring statments has been visited
		std::unordered_set<int>::const_iterator exist = visitedStmts.find(stmts);

		//If neighbouring statments has not been visited, visit it.
		if (exist == visitedStmts.end()) {
			DFSRecursivePrevious(stmts, visitedStmts,stmtsBefore);
		}
	}
}