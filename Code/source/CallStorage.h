#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

#include "Hasher.h"

/*
	A structure to contain:	the procedure calling another (caller),
							a list of procedures it is calling (callee),
							a list of procedures that is directly/indirectly calling (callAnc),
							a list of procedures being directly/indirectly called (callDesc)
	If such statement or list does not exist, it'll be set to 0 or {} respectively
*/
struct cRelationships
{
	unordered_set<string> caller;
	unordered_set<string> callees;
	unordered_set<string> callAnc;
	unordered_set<string> callDesc;
};

/*
	A storage class to store the Call & Call* relationships of statements
*/
class CallStorage {
public:
	CallStorage();

	/*
		Adds the call relation into the various lists in the storage
		Returns false if the pair already exist
	*/
	bool addCall(string caller, string callee);

	/*
		Sets "callAnc" of callee
		Each callAnc - callee pair is entered into callStarPairList
		If callee already has a list of callAnc, it is not replaced and it return false
	*/
	bool setCallAnc(string callee, unordered_set<string> callers);

	/*
		Sets "callDesc" of caller
		Each caller - callDesc pair is entered into callStarPairList
		If caller already has a list of callDesc, it is not replaced and it return false
	*/
	bool setCallDesc(string caller, unordered_set<string> callees);

	// returns true if callTable is empty
	bool isEmpty();

	// returns true if call* pair is found
	bool hasCallStarPair(pair<string, string> pair);

	// returns true if the specified procedure calls another
	bool isCaller(string procedure);

	// returns true if the specified procedure is called by another
	bool isCallee(string procedure);

	/*
		return the procedure calling the procedure specified
		return "" if 'procedure' is not found
	*/
	unordered_set<string> getCaller(string procedure);

	/*
		return the procedure called by the procedure specified
		return {} if 'procedure' is not found
	*/
	unordered_set<string> getCallee(string procedure);

	/*
		return a list of procedures that is directly/indirectly calling the procedure specified
		return an empty set if 'procedure' is not found
	*/
	unordered_set<string> getCallAnc(string procedure);

	/*
		return a list of procedures that is directly/indirectly called by the procedure specified
		return an empty set if 'procedure' is not found
	*/
	unordered_set<string> getCallDesc(string procedure);

	// returns a list of all procedures that calls another
	unordered_set<string> getAllCallers();

	// returns a list of all procedures that is called by another
	unordered_set<string> getAllCallees();

	// returns a list of all call pairs
	unordered_set< pair<string, string>, strPairhash> getCallPairs();

	// returns a list of all call* pairs
	unordered_set< pair<string, string>, strPairhash> getCallStarPairs();

private:
	static unordered_map<string, cRelationships> callTable;
	static unordered_set< pair<string, string>, strPairhash> callPairList;
	static unordered_set< pair<string, string>, strPairhash> callStarPairList;
	static unordered_set<string> callerList;
	static unordered_set<string> calleeList;
};