#include"RunTimeDesignExtractor.h"

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
	int stmtNum = pkb->getTotalStmNo();
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
	unordered_set<int> adjacentStmts = pkb->getNext(statments);

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
	unordered_set<int> adjacentStmts = pkb->getPrev(statments);

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




//Get true/false of Affects(int, int)
bool RunTimeDesignExtractor::isAffect(int stmt, int stmt1) {
	//Check if stmt is able to affect stmt1.
	bool isPossible = isAffectPossible(stmt, stmt1);

	if (isPossible) {
		//If possible, we do a CFG check.
		std::unordered_set<int> path;
		return DFSRecursiveCheckAffectsPair(stmt, stmt1, stmt, path, true);
	}
	else {
		return false;
	}
}

bool RunTimeDesignExtractor::DFSRecursiveCheckAffectsPair(int start, int target, int current, std::unordered_set<int> &cfgPath, bool isStart) {

	//If is not starting node we will not check if it affects or breaks affects
	//Subsequently, if we visit the starting node again we will go into this clause.
	if (!isStart) {
		// We have reached target, and we know it is possible to Affect
		if (current == target) {
			return true;
		}
		//We have encountered something in this path that breaks Affects. Therefore we return false.
		else if (isLastModifiedBroken(current, start)) {
			return false;
		}
	}

	//Add stmt to CFGPath if is start of DFS
	//Subsequently it will always add to cfgPath
	if (!isStart) {
		cfgPath.insert(current);
	}

	//Get neighbouring next statements
	std::unordered_set<int> nextStatements = pkb->getNext(current);

	//For each neighbouring procedure
	for (int nextStmt : nextStatements) {

		//Check if next Statemnt is inside the CFGPath.
		std::unordered_set<int>::const_iterator currPath = cfgPath.find(nextStmt);

		//If next statement has not been visited, visit it.
		if (currPath == cfgPath.end()) {
			bool result = DFSRecursiveCheckAffectsPair(start, target, nextStmt, cfgPath, false);
			if (result) {
				return true;
			}
		}
	}
	return false;
}

//Get true/false of Affects(int,_)
bool RunTimeDesignExtractor::isStatementAffectingAnother(int stmt) {
	if (pkb->getStmType(stmt) != stmType::assign) {
		return false;
	}
	std::unordered_set<int> cfgPath;
	return DFSRecursiveCheckAffecting(stmt, stmt, cfgPath, true);
}

bool RunTimeDesignExtractor::DFSRecursiveCheckAffecting(int start, int current, std::unordered_set<int> &cfgPath, bool isStart) {
	//If is not starting node we will not check if it affects or breaks affects
	//Subsequently, if we visit the starting node again we will go into this clause.
	if (!isStart) {
		if (isAffectPossible(start, current)) {
			return true;
		}
		//We have encountered something in this path that breaks Affects. Therefore we return false.
		else if (isLastModifiedBroken(current, start)) {
			return false;
		}
	}

	//Add stmt to CFGPath if is start of DFS
	//Subsequently it will always add to cfgPath
	if (!isStart) {
		cfgPath.insert(current);
	}

	//Get neighbouring next statements
	std::unordered_set<int> nextStatements = pkb->getNext(current);

	//For each neighbouring procedure
	for (int nextStmt : nextStatements) {

		//Check if next Statemnt is inside the CFGPath.
		std::unordered_set<int>::const_iterator currPath = cfgPath.find(nextStmt);

		//If next statement has not been visited, visit it.
		if (currPath == cfgPath.end()) {
			bool result = DFSRecursiveCheckAffecting(start, nextStmt, cfgPath, false);
			if (result) {
				return true;
			}
		}
	}
	return false;
}

//Get true/false of Affects (_,int)
bool RunTimeDesignExtractor::isStatementAffectedByAnother(int stmt) {
	if (pkb->getStmType(stmt) != stmType::assign) {
		return false;
	}
	std::unordered_set<int> cfgPath;
	return DFSRecursiveCheckAffectedBy(stmt, stmt, cfgPath, true);
}

bool RunTimeDesignExtractor::DFSRecursiveCheckAffectedBy(int end, int current, std::unordered_set<int> &cfgPath, bool isStart) {
	//If is not starting node we will not check if it affects or breaks affects
	//Subsequently, if we visit the starting node again we will go into this clause.
	if (!isStart) {
		if (isAffectPossible(current, end)) {
			return true;
		}
	}

	//Add stmt to CFGPath if is start of DFS
	//Subsequently it will always add to cfgPath
	if (!isStart) {
		cfgPath.insert(current);
	}

	//Get neighbouring prev statements
	std::unordered_set<int> prevStatements = pkb->getPrev(current);

	//For each neighbouring prev statements
	for (int prevStmt : prevStatements) {

		//Check if prev Statemnt is inside the CFGPath.
		std::unordered_set<int>::const_iterator currPath = cfgPath.find(prevStmt);

		//If next statement has not been visited, visit it.
		if (currPath == cfgPath.end()) {
			bool result = DFSRecursiveCheckAffecting(end, prevStmt, cfgPath, false);
			if (result) {
				return true;
			}
		}
	}
	return false;
}

//Get true/false of Affects(_,_)
bool RunTimeDesignExtractor::hasAffectsRelation() {
	int stmNumber = pkb->getTotalStmNo();

	for (int i = 0; i < stmNumber; i++) {
		if (isStatementAffectingAnother(i)) {
			return true;
		}
	}
	return false;
}

//Get list of s where Affects(s, index)
vector<int> RunTimeDesignExtractor::getStatementsAffectingIndex(int stmt) {
	if (pkb->getStmType(stmt) != stmType::assign) {
		return vector<int>();
	}
	std::unordered_set<int> cfgPath;
	std::vector<int> affectedByList;
	DFSRecursiveGetAffectingList(stmt, stmt, cfgPath, true, affectedByList);
}

void RunTimeDesignExtractor::DFSRecursiveGetAffectedByList(int end, int current, std::unordered_set<int> &cfgPath, bool isStart, std::vector<int> &affectedByList, std::unordered_set<std::string> &relevantVar) {
	//If is not starting node we will not check if it affects or breaks affects
	//Subsequently, if we visit the starting node again we will go into this clause.
	//If we find that affects is possible we return as we know anything above it in the CFG cannot affect the end statemnt.
	std::string currentModified;

	if (!isStart) {
		if (isAffectPossible(current, end)) {
			std::unordered_set<std::string> modifiedVar = pkb->getVarModifiedByStm(current);
			//Should only have 1 modified variable.
			for (std::string var : modifiedVar) {
				currentModified = var;
				relevantVar.insert(var);
			}

			std::unordered_set<std::string>::const_iterator alreadyModified = relevantVar.find(currentModified);
			//Not found
			if (alreadyModified == relevantVar.end()) {
				affectedByList.push_back(current);
			}
		}
	}

	//Early return if the var used by statement has already all been found.
	if (pkb->getVarUsedByStm(end) == relevantVar) {
		relevantVar.erase(currentModified);
		return;
	}

	//Add stmt to CFGPath if is not start of DFS
	//Subsequently it will always add to cfgPath
	if (!isStart) {
		cfgPath.insert(current);
	}

	//Get neighbouring next statements
	std::unordered_set<int> prevStatements = pkb->getPrev(current);

	//For each neighbouring procedure
	for (int prevStmt : prevStatements) {

		//Check if next Statemnt is inside the CFGPath.
		std::unordered_set<int>::const_iterator currPath = cfgPath.find(prevStmt);

		//If next statement has not been visited, visit it.
		if (currPath == cfgPath.end()) {
			DFSRecursiveGetAffectedByList(end, prevStmt, cfgPath, false, affectedByList, relevantVar);
		}
	}

	relevantVar.erase(currentModified);
	return;
}

//Get list of s where Affects(index, s)
vector<int> RunTimeDesignExtractor::getStatementsAffectedByIndex(int stmt) {
	if (pkb->getStmType(stmt) != stmType::assign) {
		return vector<int>();
	}
	std::unordered_set<int> cfgPath;
	std::vector<int> affectedList;
	DFSRecursiveGetAffectingList(stmt, stmt, cfgPath, true, affectedList);
}

void RunTimeDesignExtractor::DFSRecursiveGetAffectingList(int start, int current, std::unordered_set<int> &cfgPath, bool isStart, std::vector<int> &affectedList) {
	//If is not starting node we will not check if it affects or breaks affects
	//Subsequently, if we visit the starting node again we will go into this clause.
	if (!isStart) {
		if (isAffectPossible(start, current)) {
			affectedList.push_back(current);
		}
		//We have encountered something in this path that breaks Affects. Therefore we return false.
		else if (isLastModifiedBroken(current, start)) {
			return;
		}
	}

	//Add stmt to CFGPath if is not start of DFS
	//Subsequently it will always add to cfgPath
	if (!isStart) {
		cfgPath.insert(current);
	}

	//Get neighbouring next statements
	std::unordered_set<int> nextStatements = pkb->getNext(current);

	//For each neighbouring procedure
	for (int nextStmt : nextStatements) {

		//Check if next Statemnt is inside the CFGPath.
		std::unordered_set<int>::const_iterator currPath = cfgPath.find(nextStmt);

		//If next statement has not been visited, visit it.
		if (currPath == cfgPath.end()) {
			DFSRecursiveGetAffectingList(start, nextStmt, cfgPath, false, affectedList);
		}
	}
	return;
}

//Get list of s where of Affects(s, _)
vector<int> RunTimeDesignExtractor::getAllStatementsAffectingAnother() {
	int stmtListSize = pkb->getTotalStmNo();
	vector<int> result;

	for (int i = 1; i <= stmtListSize; i++) {
		if (isStatementAffectingAnother(i)) {
			result.push_back(i);
		}
	}
	return result;
}

//Get list of s where of Affects(_, s)
vector<int> RunTimeDesignExtractor::getAllStatementsAffectingByAnother() {
	int stmtListSize = pkb->getTotalStmNo();
	vector<int> result;

	for (int i = 1; i <= stmtListSize; i++) {
		if (isStatementAffectedByAnother(i)) {
			result.push_back(i);
		}
	}
	return result;
}

//Get pairs of s where Affects(s, s)
std::unordered_set<pair<int, int>> RunTimeDesignExtractor::getAffectsPair() {
	std::unordered_set<std::string> procList = pkb->getProcList();
	std::unordered_set<pair<int, int>> result;
	for (std::string procedure : procList) {
		std::unordered_set<pair<int, int>> subResult = getAffectsPairOfProc(procedure);
		for (pair<int, int> affectsPair : subResult) {
			result.insert(affectsPair);
		}
	}
	return result;
}

std::unordered_set<pair<int, int>> RunTimeDesignExtractor::getAffectsPairOfProc(std::string procedure) {
	std::vector<int> stmList = pkb->getStmList(procedure);

	int start = stmList.front();

	std::unordered_map<std::string, unordered_set<int>> lastModifiedTable;
	std::unordered_set<pair<int, int>> pairList;

	extractAffectsPair(start, lastModifiedTable, pairList);

	return pairList;
}

void RunTimeDesignExtractor::extractAffectsPair(int start, std::unordered_map<std::string, std::unordered_set<int>> &lastModifiedTable, std::unordered_set<pair<int, int>> &affectsPair) {
	for (int i = start; i == 0; i = pkb->getFollower(i)) {
		if (pkb->getStmType(i) == stmType::whileStm) {
			processWhile(lastModifiedTable, i, affectsPair);
		}
		else if (pkb->getStmType(i) == stmType::ifStm) {
			processIfStatement(lastModifiedTable, i, affectsPair);
		}
		else if (pkb->getStmType(i) == stmType::read || pkb->getStmType(i) == stmType::call) {
			processCallAndRead(i, lastModifiedTable);
		}
		else if (pkb->getStmType(i) == stmType::assign) {
			processAssign(i, lastModifiedTable, affectsPair);
		}
	}
}

void RunTimeDesignExtractor::processWhile(std::unordered_map<std::string, std::unordered_set<int>> & lastModifiedTable, int &i, std::unordered_set<std::pair<int, int>> & affectsPair)
{
	//Make copy for While Block
	std::unordered_map<std::string, std::unordered_set<int>> lastModifiedTableCopy = lastModifiedTable;

	int whileStatementFirst = pkb->getWhileStmContainer(i).front();

	extractAffectsPair(whileStatementFirst, lastModifiedTable, affectsPair);

	//If the modifiedTable was updated, we redo the while loop.
	while (lastModifiedTableCopy != lastModifiedTable) {
		lastModifiedTableCopy = lastModifiedTable;
		extractAffectsPair(whileStatementFirst, lastModifiedTable, affectsPair);
	}
}

void RunTimeDesignExtractor::processIfStatement(std::unordered_map<std::string, std::unordered_set<int>> & lastModifiedTable, int &i, std::unordered_set<std::pair<int, int>> & affectsPair)
{
	//Make copy for if Block
	std::unordered_map<std::string, std::unordered_set<int>> lastModifiedTableCopy1 = lastModifiedTable;
	//Make copy for else Block
	std::unordered_map<std::string, std::unordered_set<int>> lastModifiedTableCopy2 = lastModifiedTable;

	int ifStatementFirst = pkb->getIfStmContainer(i).front();
	int elseStatementFirst = pkb->getElseStmContainer(i).front();

	//Parse if Side
	extractAffectsPair(ifStatementFirst, lastModifiedTableCopy1, affectsPair);

	//Parse Else side
	extractAffectsPair(elseStatementFirst, lastModifiedTableCopy2, affectsPair);

	//Merge the 2 lastModified Table
	for (std::pair <std::string, std::unordered_set<int>> entry : lastModifiedTableCopy1) {
		std::string var = entry.first;
		std::unordered_set<int> list = entry.second;

		//If not found, we just add the list directly to copy 2 from copy 1.
		if (lastModifiedTableCopy2.find(var) == lastModifiedTableCopy2.end()) {
			lastModifiedTableCopy2[var] = list;
		}

		//If found, we add entries from copy 1 to copy 2.
		else {
			std::unordered_set<int> stmtList = lastModifiedTableCopy2[var];
			for (int stmt : list) {
				stmtList.insert(stmt);
			}
			lastModifiedTableCopy2[var] = stmtList;
		}
	}

	lastModifiedTable = lastModifiedTableCopy2;
}

void RunTimeDesignExtractor::processAssign(int &i, std::unordered_map<std::string, std::unordered_set<int>> & lastModifiedTable, std::unordered_set<std::pair<int, int>> & affectsPair)
{
	std::unordered_set<std::string> usedList = pkb->getVarUsedByStm(i);
	std::unordered_set<std::string> modifiedList = pkb->getVarModifiedByStm(i);

	//Check if Affects
	for (std::string usedVar : usedList) {
		std::unordered_map<std::string, std::unordered_set<int>>::const_iterator got = lastModifiedTable.find(usedVar);

		//If inside lastModified table
		if (got != lastModifiedTable.end()) {
			for (int first : got->second) {
				affectsPair.insert(std::make_pair(first, i));
			}
		}
	}

	//Update Last Modified Table.
	for (std::string modifiedVar : modifiedList) {
		//if is new var
		if (lastModifiedTable.find(modifiedVar) == lastModifiedTable.end()) {
			std::unordered_set<int> stmtList;
			stmtList.insert(i);
			lastModifiedTable[modifiedVar] = stmtList;
		}
		//if is old var already existing.
		else {
			std::unordered_set<int> stmtList = lastModifiedTable[modifiedVar];
			stmtList.insert(i);
			lastModifiedTable[modifiedVar] = stmtList;
		}
	}
}

void RunTimeDesignExtractor::processCallAndRead(int &i, std::unordered_map<std::string, std::unordered_set<int>> & lastModifiedTable)
{
	std::unordered_set<std::string> modifiedList = pkb->getVarModifiedByStm(i);
	//Update Last Modified Table.
	for (std::string modifiedVar : modifiedList) {
		//if is new var
		if (lastModifiedTable.find(modifiedVar) == lastModifiedTable.end()) {
			std::unordered_set<int> stmtList;
			stmtList.insert(i);
			lastModifiedTable[modifiedVar] = stmtList;
		}
		//if is old var already existing.
		else {
			std::unordered_set<int> stmtList = lastModifiedTable[modifiedVar];
			stmtList.insert(i);
			lastModifiedTable[modifiedVar] = stmtList;
		}
	}
}


//Get list of s where Affects*(int, s)
vector<int> RunTimeDesignExtractor::getAllStatementsAffectedByIndexStar(int index) {
	std::string procedure = pkb->getProcOfStm(index);
	std::unordered_set<pair<int, int>> relevantPairs = getAffectsPairOfProc(procedure);
	std::unordered_map<int, std::unordered_set<int>> adjacencyList;
	std::vector<int> results;
	
	for (pair<int, int> affectPair : relevantPairs) {
		std::unordered_set<int> adjacents = adjacencyList[affectPair.first];
		adjacents.insert(affectPair.second);
		adjacencyList[affectPair.first] = adjacents;
	}

	std::unordered_set<int> visitedPath;
	DFSRecursiveReachability(index, results, visitedPath, adjacencyList) {
	}
}

void DFSRecursiveReachability(int start, std::vector<int>& results, std::unordered_set<int>& visitedPath, std::unordered_map<int, std::unordered_set<int>> adjacencyList) {
	visitedPath.insert(start);

	std::unordered_set<int> adjacentStms = adjacencyList[start];

	for (int adjacentStm : adjacentStms) {
		results.push_back(adjacentStm);

		DFSRecursiveReachability(adjacentStm, results, visitedPath, adjacencyList);
	}
}
//Get list of s where Affects*(s, int)
vector<int> RunTimeDesignExtractor::getAllStatementsAffectingIndexStar() {

}

bool RunTimeDesignExtractor::isAffectPossible(int stmt, int stmt1) {
	//Check if stmt and stmt1 are Assign Statements
	if (pkb->getStmType(stmt) != stmType::assign) {
		return false;
	}
	else if (pkb->getStmType(stmt1) != stmType::assign) {
		return false;
	}

	//Check if stmt modifies something that stmt1 uses
	std::unordered_set<std::string> modifiedInStmt = pkb->getVarModifiedByStm(stmt);
	std::unordered_set<std::string> usedInStmt1 = pkb->getVarModifiedByStm(stmt1);
	bool isPossible = contains(modifiedInStmt, usedInStmt1);

	return isPossible;
}

bool RunTimeDesignExtractor::isLastModifiedBroken(int current, int start)
{
	stmType type = pkb->getStmType(current);
	if (type == stmType::assign || type == stmType::read || type == stmType::call) {
		std::unordered_set<std::string> modifiedVar = pkb->getVarModifiedByStm(start);
		std::unordered_set<std::string> modifiedInCurrent = pkb->getVarModifiedByStm(current);
		bool found = contains(modifiedVar, modifiedInCurrent);
		return found;
	}
	return false;
}

//Check if modified var can be found in var used in statement.
bool RunTimeDesignExtractor::contains(std::unordered_set<std::string> &modifiedInStmt, std::unordered_set<std::string> &usedInStmt1)
{
	for (std::string modified : modifiedInStmt) {
		for (std::string used : usedInStmt1) {
			if (modified == used) {
				return true;
			}
		}
	}
	return false;
}