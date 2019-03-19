#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>

using namespace std;

#include "Hasher.h"

/*
	A storage class to store the Uses relationships between Procedures & Statements with Variables
*/
class UseStorage
{
public:
	UseStorage();

	bool addUsesStm(int stm, string variable);
	bool addUsesProc(string procedure, string variable);

	bool containsStmVarPair(pair<int, string> pair);
	bool containsProcVarPair(pair<string, string> pair);
	unordered_set<string> getVarUsedByStm(int stm);
	unordered_set<string> getVarUsedByProc(string proc);
	unordered_set<int> getStmUsing(string variable);
	unordered_set<string> getProcUsing(string variable);
	unordered_set< pair<int, string>, intStringhash> getStmVarPairs();
	unordered_set< pair<string, string>, strPairhash> getProcVarPairs();

private:
	static unordered_set<pair<int, string>, intStringhash> stmVarPairList;
	static unordered_set<pair<string, string>, strPairhash> procVarPairList;
	static unordered_map<int, unordered_set<string> > stmToVarMap;
	static unordered_map<string, unordered_set<string> > procToVarMap;
	static unordered_map<string, unordered_set<int> > varToStmMap;
	static unordered_map<string, unordered_set<string> > varToProcMap;
};
