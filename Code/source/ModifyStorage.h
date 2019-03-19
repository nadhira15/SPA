#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>

using namespace std;

#include "Hasher.h"

/*
	A storage class to store the Modifies relationships between Procedures & Statements with Variables
*/
class ModifyStorage
{
public:
	ModifyStorage();

	bool addModifiesStm(int stm, string variable);
	bool addModifiesProc(string procedure, string variable);

	bool containsStmVarPair(pair<int, string> pair);
	bool containsProcVarPair(pair<string, string> pair);
	unordered_set<string> getVarModifiedByStm(int stm);
	unordered_set<string> getVarModifiedByProc(string proc);
	unordered_set<int> getStmModifying(string variable);
	unordered_set<string> getProcModifying(string variable);
	unordered_set< pair<int, string>, intStringhash> getStmVarPairs();
	unordered_set< pair<string, string>, strPairhash> getProcVarPairs();

private:
	static unordered_set<pair<int, string>, intStringhash> stmVarPairList;
	static unordered_set<pair<string, string>, strPairhash> procVarPairList;
	static unordered_map<int, unordered_set<string>> stmToVarMap;
	static unordered_map<string, unordered_set<string>> procToVarMap;
	static unordered_map<string, unordered_set<int> > varToStmMap;
	static unordered_map<string, unordered_set<string> > varToProcMap;
};