#include "UseStorage.h"

unordered_set<pair<int, string>, intStringhash> UseStorage::stmVarPairList;
unordered_set<pair<string, string>, strPairhash> UseStorage::procVarPairList;
unordered_map<int, unordered_set<string> > UseStorage::stmToVarMap;
unordered_map<string, unordered_set<string> > UseStorage::procToVarMap;
unordered_map<string, unordered_set<int> > UseStorage::varToStmMap;
unordered_map<string, unordered_set<string> > UseStorage::varToProcMap;

UseStorage::UseStorage()
{
}

bool UseStorage::addUsesStm(int stm, string variable)
{
	if (!stmVarPairList.emplace(pair<int, string>(stm, variable)).second)
	{
		return false;
	}
	if (!stmToVarMap.emplace(stm, unordered_set<string>{ variable }).second)
	{
		stmToVarMap.at(stm).emplace(variable);
	}
	if (!varToStmMap.emplace(variable, unordered_set<int>{ stm }).second)
	{
		varToStmMap.at(variable).emplace(stm);
	}
	if (!varToStmMap.emplace("", unordered_set<int>{ stm }).second)
	{
		varToStmMap.at("").emplace(stm);
	}
	return true;
}

bool UseStorage::addUsesProc(string procedure, string variable)
{
	if (!procVarPairList.emplace(pair<string, string>(procedure, variable)).second)
	{
		return false;
	}
	if (!procToVarMap.emplace(procedure, unordered_set<string>{ variable }).second)
	{
		procToVarMap.at(procedure).emplace(variable);
	}
	if (!varToProcMap.emplace(variable, unordered_set<string>{ procedure }).second)
	{
		varToProcMap.at(variable).emplace(procedure);
	}
	if (!varToProcMap.emplace("", unordered_set<string>{ procedure }).second)
	{
		varToProcMap.at("").emplace(procedure);
	}
	return true;
}

bool UseStorage::containsStmVarPair(pair<int, string> pair)
{
	return stmVarPairList.find(pair) != stmVarPairList.end();
}

bool UseStorage::containsProcVarPair(pair<string, string> pair)
{
	return procVarPairList.find(pair) != procVarPairList.end();
}

unordered_set<string> UseStorage::getVarUsedByStm(int stm)
{
	if (stmToVarMap.find(stm) != stmToVarMap.end())
	{
		return stmToVarMap.at(stm);
	}
	return {};
}

unordered_set<string> UseStorage::getVarUsedByProc(string procedure)
{
	if (procToVarMap.find(procedure) != procToVarMap.end())
	{
		return procToVarMap.at(procedure);
	}
	return {};
}

unordered_set<int> UseStorage::getStmUsing(string variable)
{
	if (varToStmMap.find(variable) != varToStmMap.end())
	{
		return varToStmMap.at(variable);
	}
	return {};
}

unordered_set<string> UseStorage::getProcUsing(string variable)
{
	if (varToProcMap.find(variable) != varToProcMap.end())
	{
		return varToProcMap.at(variable);
	}
	return {};
}

unordered_set<pair<int, string>, intStringhash> UseStorage::getStmVarPairs()
{
	return stmVarPairList;
}

unordered_set<pair<string, string>, strPairhash> UseStorage::getProcVarPairs()
{
	return procVarPairList;
}

void UseStorage::erase()
{
	stmVarPairList.erase(stmVarPairList.begin(), stmVarPairList.end());
	procVarPairList.erase(procVarPairList.begin(), procVarPairList.end());
	stmToVarMap.erase(stmToVarMap.begin(), stmToVarMap.end());
	procToVarMap.erase(procToVarMap.begin(), procToVarMap.end());
	varToStmMap.erase(varToStmMap.begin(), varToStmMap.end());
	varToProcMap.erase(varToProcMap.begin(), varToProcMap.end());
}
