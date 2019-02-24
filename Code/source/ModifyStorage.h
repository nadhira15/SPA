#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>

using namespace std;

class ModifyStorage
{
public:
	ModifyStorage();

	bool addModifies(int stm, string variable);
	bool addModifies(string procedure, string variable);

	bool containsStmVarPair(pair<int, string> pair);
	bool containsProcVarPair(pair<string, string> pair);
	string getVarModifiedBy(int stm);
	string getVarModifiedBy(string proc);
	unordered_set<int> getStmModifying(string variable);
	unordered_set<string> getProcModifying(string variable);
	unordered_set< pair<int, string> > getStmVarPairs();
	unordered_set< pair<string, string> > getProcVarPairs();

private:
	static unordered_set<pair<int, string> > stmVarPairs;
	static unordered_set<pair<string, string> > procVarPairs;
	static unordered_map<int, string> varList_Stm;
	static unordered_map<string, string> varList_Proc;
	static unordered_map<string, unordered_set<int> > stmLists;
	static unordered_map<string, unordered_set<string> > procLists;
};