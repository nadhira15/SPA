#include "CallStorage.h"

unordered_map<string, cRelationships> CallStorage::callTable;
unordered_set< pair<string, string>, strPairhash> CallStorage::callPairList;
unordered_set< pair<string, string>, strPairhash> CallStorage::callStarPairList;
unordered_set<string> CallStorage::callerList;
unordered_set<string> CallStorage::calleeList;
unordered_map<int, string> CallStorage::stmToProcMap;
unordered_map<string, unordered_set<int>> CallStorage::procToStmMap;

CallStorage::CallStorage()
{
}

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

bool CallStorage::addCall(string caller, string callee, int stm)
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

	// if callee exist in procToStmMap
	if (!procToStmMap.emplace(callee, unordered_set<int>{ stm }).second)
	{
		procToStmMap.find(callee)->second.emplace(stm);
	}

	// if caller exist in callTable
	if (!callTable.emplace(caller, cRelationships{ {} ,{callee}, {}, {} }).second)
	{
		callTable.find(caller)->second.callees.emplace(callee);
	}

	callerList.emplace(caller);
	calleeList.emplace(callee);
	stmToProcMap.emplace(stm, callee);
	return true;
}

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

unordered_set<string> CallStorage::getCaller(string procedure)
{
	if (callTable.find(procedure) != callTable.end())
	{
		return callTable.at(procedure).caller;
	}
	return {};
}

unordered_set<string> CallStorage::getCallee(string procedure)
{
	if (callTable.find(procedure) != callTable.end())
	{
		return callTable.at(procedure).callees;
	}
	return {};
}

unordered_set<string> CallStorage::getCallAnc(string procedure)
{
	if (callTable.find(procedure) != callTable.end())
	{
		return callTable.at(procedure).callAnc;
	}
	return {};
}

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

string CallStorage::getProcCalledBy(int stm)
{
	if (stmToProcMap.find(stm) != stmToProcMap.end())
	{
		return stmToProcMap.at(stm);
	}
	return "";
}

unordered_set<int> CallStorage::getStmCalling(string procedure)
{
	if (procToStmMap.find(procedure) != procToStmMap.end())
	{
		return procToStmMap.at(procedure);
	}
	return {};
}
