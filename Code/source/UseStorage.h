#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>

using namespace std;

#include "Hasher.h"

class UseStorage
{
public:
	UseStorage();

	bool addUses(int stm, string variable);
	bool addUses(string procedure, string variable);

	bool containsStmVarPair(pair<int, string> pair);
	bool containsProcVarPair(pair<string, string> pair);
	unordered_set<string> getVarUsedBy(int stm);
	unordered_set<string> getVarUsedBy(string proc);
	unordered_set<int> getStmUsing(string variable);
	unordered_set<string> getProcUsing(string variable);
	unordered_set< pair<int, string>, intStringhash> getStmVarPairs();
	unordered_set< pair<string, string>, strPairhash> getProcVarPairs();

private:
	static unordered_set<pair<int, string>, intStringhash> stmVarPairs;
	static unordered_set<pair<string, string>, strPairhash> procVarPairs;
	static unordered_map<int, unordered_set<string> > varLists_Stm;
	static unordered_map<string, unordered_set<string> > varLists_Proc;
	static unordered_map<string, unordered_set<int> > stmLists;
	static unordered_map<string, unordered_set<string> > procLists;
};
