#include "CallStorage.h"

unordered_map<string, cRelationships> CallStorage::callTable;
unordered_set< pair<string, string>, strPairhash> CallStorage::callPairList;
unordered_set< pair<string, string>, strPairhash> CallStorage::callStarPairList;
unordered_set<string> CallStorage::callerList;
unordered_set<string> CallStorage::calleeList;

CallStorage::CallStorage()
{
}

/*
	Adds the call relation into the various lists in the storage
	Returns false if the pair already exist
*/
bool CallStorage::addCall(string caller, string callee)
{
	// if Call Pair is already added
	if (!callPairList.emplace(pair<string, string>(caller, callee)).second)
	{
		return false;
	}

	// if callee exist in callTable
	if (!callTable.emplace(callee, cRelationships{ {caller}, {}, {}, {} }).second)
	{
		callTable.find(callee)->second.caller.emplace(caller);
	}

	// if caller exist in callTable
	if (!callTable.emplace(caller, cRelationships{ {} ,{callee}, {}, {} }).second)
	{
		callTable.find(caller)->second.callees.emplace(callee);
	}

	callerList.emplace(caller);
	calleeList.emplace(callee);
	return true;
}

/*
	Sets "callAnc" of callee
	Each callAnc - callee pair is entered into callStarPairList
	If callee already has a list of callAnc, it is not replaced and it return false
*/
bool CallStorage::setCallAnc(string callee, unordered_set<string> callAnc)
{
	if (callTable.find(callee)->second.callAnc.size() != 0)
	{
		return false;
	}

	callTable.find(callee)->second.callAnc = callAnc;

	for (auto itr = callAnc.cbegin(); itr != callAnc.cend(); ++itr)
	{
		callStarPairList.emplace(pair<string, string>(*itr, callee));
	}
	return true;
}

/*
	Sets "callDesc" of caller
	Each caller - callDesc pair is entered into callStarPairList
	If caller already has a list of callDesc, it is not replaced and it return false
*/
bool CallStorage::setCallDesc(string caller, unordered_set<string> callDesc)
{
	if (callTable.find(caller)->second.callDesc.size() != 0)
	{
		return false;
	}

	callTable.find(caller)->second.callDesc = callDesc;

	for (auto itr = callDesc.cbegin(); itr != callDesc.cend(); ++itr)
	{
		callStarPairList.emplace(pair<string, string>(caller, *itr));
	}
	return true;
}

bool CallStorage::isEmpty()
{
	return callTable.size() == 0;
}

// returns true if call* pair is found
bool CallStorage::hasCallStarPair(pair<string, string> pair)
{
	return callStarPairList.find(pair) != callStarPairList.end();
}

bool CallStorage::isCaller(string procedure)
{
	return callerList.find(procedure) != callerList.end();
}

bool CallStorage::isCallee(string procedure)
{
	return calleeList.find(procedure) != calleeList.end();
}

/*
	return the procedure calling the procedure specified
	return "" if 'procedure' is not found
*/
unordered_set<string> CallStorage::getCaller(string procedure)
{
	if (callTable.find(procedure) != callTable.end())
	{
		return callTable.at(procedure).caller;
	}
	return {};
}

/*
	return the procedure called by the procedure specified
	return {} if 'procedure' is not found
*/
unordered_set<string> CallStorage::getCallee(string procedure)
{
	if (callTable.find(procedure) != callTable.end())
	{
		return callTable.at(procedure).callees;
	}
	return {};
}

/*
	return a list of procedures that is directly/indirectly calling the procedure specified
	return an empty set if 'procedure' is not found
*/
unordered_set<string> CallStorage::getCallAnc(string procedure)
{
	if (callTable.find(procedure) != callTable.end())
	{
		return callTable.at(procedure).callAnc;
	}
	return {};
}

/*
	return a list of procedures that is directly/indirectly called by the procedure specified
	return an empty set if 'procedure' is not found
*/
unordered_set<string> CallStorage::getCallDesc(string procedure)
{
	if (callTable.find(procedure) != callTable.end())
	{
		return callTable.at(procedure).callDesc;
	}
	return {};
}

unordered_set<string> CallStorage::getAllCallees()
{
	return calleeList;
}

unordered_set<string> CallStorage::getAllCallers()
{
	return callerList;
}

unordered_set<pair<string, string>, strPairhash> CallStorage::getCallPairs()
{
	return callPairList;
}

unordered_set<pair<string, string>, strPairhash> CallStorage::getCallStarPairs()
{
	return callStarPairList;
}
