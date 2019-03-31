#include"RunTimeDesignExtractor.h"
#include"PKB.h"

PKB RunTimeDesignExtractor::pkb;

unordered_set<int> RunTimeDesignExtractor::extractNextStar(int stmt) {
	unordered_set<int>visitedStmts;
	vector<int>sortedStmts;
	DFSRecursiveNext(stmt, visitedStmts);
	return visitedStmts;
}

unordered_set<int> RunTimeDesignExtractor::extractPreviousStar(int stmt) {
	unordered_set<int>visitedStmts;
	DFSRecursivePrevious(stmt, visitedStmts);
	return visitedStmts;
}

bool RunTimeDesignExtractor::extractNextStarPair(int start, int end) {
	unordered_set<int>visitedStmts;
	DFSRecursiveNext(start, visitedStmts);
	if (visitedStmts.find(end) == visitedStmts.end()) {
		return false;
	}
	return true;
}

void RunTimeDesignExtractor::DFSRecursiveNext(int statments, unordered_set<int> &visitedStmts) {
	//Marks statment as visited
	visitedStmts.insert(statments);
	//Get neighbouring statments.
	unordered_set<int> adjacentStmts = pkb.getNext(statments);

	//For each neighbouring statments
	for (int stmts : adjacentStmts) {

		//Check if neighbouring statments has been visited
		std::unordered_set<int>::const_iterator exist = visitedStmts.find(stmts);

		//If neighbouring statments has not been visited, visit it.
		if (exist == visitedStmts.end()) {
			DFSRecursiveNext(stmts, visitedStmts);
		}
	}
}

void RunTimeDesignExtractor::DFSRecursivePrevious(int statments, unordered_set<int> &visitedStmts) {
	//Marks statment as visited
	visitedStmts.insert(statments);
	//Get neighbouring statments.
	unordered_set<int> adjacentStmts = pkb.getPrev(statments);

	//For each neighbouring statments
	for (int stmts : adjacentStmts) {

		//Check if neighbouring statments has been visited
		std::unordered_set<int>::const_iterator exist = visitedStmts.find(stmts);

		//If neighbouring statments has not been visited, visit it.
		if (exist == visitedStmts.end()) {
			DFSRecursivePrevious(stmts, visitedStmts);
		}
	}
}