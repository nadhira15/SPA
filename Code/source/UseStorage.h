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

	/*
		add the Uses relation for a statement into the relevant lists and maps in the storage
		Returns false if the pair is already exist
	*/
	bool addUsesStm(int stm, string variable);

	/*
		add the Uses relation for a procedure into the relevant lists and maps in the storage
		Returns false if the pair is already exist
	*/
	bool addUsesProc(string procedure, string variable);

	// returns true if the specified <statement, variable> pair is found
	bool containsStmVarPair(pair<int, string> pair);

	// returns true if the specified <procedure, variable> pair is found
	bool containsProcVarPair(pair<string, string> pair);

	/*
		return a list of variables that is used by 'stm'
		return an empty set if 'stm' is not found
	*/
	unordered_set<string> getVarUsedByStm(int stm);

	/*
		return a list of variables that is used by 'procedure'
		return an empty set if 'procedure' is not found
	*/
	unordered_set<string> getVarUsedByProc(string proc);

	/*
		return a list of statements that is using 'variable'
		returns a list of all statements that used a variable if 'variable' == "" 
		return an empty set if 'variable' is not found
	*/
	unordered_set<int> getStmUsing(string variable);

	/*
		return a list of procedures that is using 'variable'
		returns a list of all procedures that used a variable if 'variable' == ""
		return an empty set if 'variable' is not found
	*/
	unordered_set<string> getProcUsing(string variable);

	// returns a list of all Uses pairs for statements
	unordered_set< pair<int, string>, intStringhash> getStmVarPairs();

	// returns a list of all Uses pairs for procedures
	unordered_set< pair<string, string>, strPairhash> getProcVarPairs();

	// empty the entire storage
	void erase();

private:
	static unordered_set<pair<int, string>, intStringhash> stmVarPairList;
	static unordered_set<pair<string, string>, strPairhash> procVarPairList;
	static unordered_map<int, unordered_set<string> > stmToVarMap;
	static unordered_map<string, unordered_set<string> > procToVarMap;
	static unordered_map<string, unordered_set<int> > varToStmMap;
	static unordered_map<string, unordered_set<string> > varToProcMap;
};
