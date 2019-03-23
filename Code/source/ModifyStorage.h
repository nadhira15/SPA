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

	/*
		add the Modifies relation for a statement into the relevant lists and maps in the storage
		Returns false if the pair is already exist
	*/
	bool addModifiesStm(int stm, string variable);

	/*
		add the Modifies relation for a procedure into the relevant lists and maps in the storage
		Returns false if the pair is already exist
	*/
	bool addModifiesProc(string procedure, string variable);

	// returns true if the specified <statement, variable> pair is found
	bool containsStmVarPair(pair<int, string> pair);

	// returns true if the specified <procedure, variable> pair is found
	bool containsProcVarPair(pair<string, string> pair);

	/*
		return the list of variables that is modified by 'stm'
		return an empty set if 'stm' is not found
	*/
	unordered_set<string> getVarModifiedByStm(int stm);

	/*
		return the list of variables that is modified by 'procedure'
		return an empty set if 'procedure' is not found
	*/
	unordered_set<string> getVarModifiedByProc(string proc);

	/*
		return the list of statements that is modifying 'variable'
		return an empty set if 'variable' is not found
	*/
	unordered_set<int> getStmModifying(string variable);

	/*
		return the list of procedures that is modifying 'variable'
		return an empty set if 'variable' is not found
	*/
	unordered_set<string> getProcModifying(string variable);

	// returns a list of all Modifies pairs for statements
	unordered_set< pair<int, string>, intStringhash> getStmVarPairs();

	// returns a list of all Modifies pairs for procedures
	unordered_set< pair<string, string>, strPairhash> getProcVarPairs();

private:
	static unordered_set<pair<int, string>, intStringhash> stmVarPairList;
	static unordered_set<pair<string, string>, strPairhash> procVarPairList;
	static unordered_map<int, unordered_set<string>> stmToVarMap;
	static unordered_map<string, unordered_set<string>> procToVarMap;
	static unordered_map<string, unordered_set<int> > varToStmMap;
	static unordered_map<string, unordered_set<string> > varToProcMap;
};