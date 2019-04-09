#pragma once
#include "Hasher.h"
#include "PKB.h"

class PKB;

class RunTimeDesignExtractor {

public:

	std::unordered_set<int> extractNextStar(int stmt);
	std::unordered_set<int> extractPreviousStar(int stmt);
	bool extractNextStarPair(int start, int end);
	std::unordered_set<std::pair<int, int>, intPairhash> getNextStarPairs();
	void DFSRecursiveNext(int procedure, std::unordered_set<int> &visitedStmts, std::unordered_set<int>& stmtsAfter);
	void DFSRecursivePrevious(int procedure, std::unordered_set<int> &visitedStmts, std::unordered_set<int> &stmtsBefore);

	bool isAffect(int stmt, int stmt1);

	bool DFSRecursiveCheckAffectsPair(int start, int target, int current, std::unordered_set<int>& cfgPath, bool isStart);

	bool isStatementAffectingAnother(int stmt);

	bool DFSRecursiveCheckAffecting(int start, int current, std::unordered_set<int>& cfgPath, bool isStart);

	bool isStatementAffectedByAnother(int stmt);

	bool DFSRecursiveCheckAffectedBy(int end, int current, std::unordered_set<int>& cfgPath, bool isStart);

	bool hasAffectsRelation();

	std::vector<int> getStatementsAffectingIndex(int stmt);

	void DFSRecursiveGetAffectedByList(int end, int current, std::unordered_set<int>& cfgPath, bool isStart, std::vector<int>& affectedByList, std::unordered_set<std::string>& relevantVar);

	std::vector<int> getStatementsAffectedByIndex(int stmt);

	void DFSRecursiveGetAffectingList(int start, int current, std::unordered_set<int>& cfgPath, bool isStart, std::vector<int>& affectedList);

	std::vector<int> getAllStatementsAffectingAnother();

	std::vector<int> getAllStatementsAffectingByAnother();

	std::unordered_set<std::pair<int, int>, intPairhash> getAffectsPair();

	std::unordered_set<std::pair<int, int>, intPairhash> getAffectsPairOfProc(std::string procedure);

	void extractAffectsPair(int start, std::unordered_map<std::string, std::unordered_set<int>>& lastModifiedTable, std::unordered_set<std::pair<int, int>, intPairhash>& affectsPair);

	void processWhile(std::unordered_map<std::string, std::unordered_set<int>> & lastModifiedTable, int &i, std::unordered_set<std::pair<int, int>, intPairhash> & affectsPair);

	void processIfStatement(std::unordered_map<std::string, std::unordered_set<int>> & lastModifiedTable, int &i, std::unordered_set<std::pair<int, int>, intPairhash> & affectsPair);

	void processAssign(int &i, std::unordered_map<std::string, std::unordered_set<int>> & lastModifiedTable, std::unordered_set<std::pair<int, int>, intPairhash> & affectsPair);

	void processCallAndRead(int &i, std::unordered_map<std::string, std::unordered_set<int>> & lastModifiedTable);

	std::vector<int> getAllStatementsAffectedByIndexStar(int index);

	std::vector<int> getAllStatementsAffectingIndexStar(int index);

	void DFSRecursiveReachability(int start, std::vector<int>& results, std::unordered_set<int>& visitedPath, std::unordered_map<int, std::unordered_set<int>> adjacencyList);

	std::unordered_set<std::pair<int, int>, intPairhash> getAffectsStarPair();

	bool isAffectPossible(int stmt, int stmt1);

	bool isLastModifiedBroken(int current, int start);

	bool contains(std::unordered_set<std::string>& modifiedInStmt, std::unordered_set<std::string>& usedInStmt1);

private:
	PKB* pkb;
};