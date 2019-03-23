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

	bool addCall(string caller, string callee);
	bool setCallAnc(string callee, unordered_set<string> callers);
	bool setCallDesc(string caller, unordered_set<string> callees);

	bool isEmpty();
	bool hasCallStarPair(pair<string, string> pair);
	bool isCaller(string procedure);
	bool isCallee(string procedure);

	unordered_set<string> getCaller(string procedure);
	unordered_set<string> getCallee(string procedure);
	unordered_set<string> getCallAnc(string procedure);
	unordered_set<string> getCallDesc(string procedure);
	unordered_set<string> getAllCallers();
	unordered_set<string> getAllCallees();
	unordered_set< pair<string, string>, strPairhash> getCallPairs();
	unordered_set< pair<string, string>, strPairhash> getCallStarPairs();

private:
	static unordered_map<string, cRelationships> callTable;
	static unordered_set< pair<string, string>, strPairhash> callPairList;
	static unordered_set< pair<string, string>, strPairhash> callStarPairList;
	static unordered_set<string> callerList;
	static unordered_set<string> calleeList;
};