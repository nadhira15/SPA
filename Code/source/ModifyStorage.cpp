#include "ModifyStorage.h"

unordered_set<pair<int, string>, intStringhash> ModifyStorage::stmVarPairList;
unordered_set<pair<string, string>, strPairhash> ModifyStorage::procVarPairList;
unordered_map<int, unordered_set<string>> ModifyStorage::stmToVarMap;
unordered_map<string, unordered_set<string>> ModifyStorage::procToVarMap;
unordered_map<string, unordered_set<int> > ModifyStorage::varToStmMap;
unordered_map<string, unordered_set<string> > ModifyStorage::varToProcMap;

ModifyStorage::ModifyStorage()
{
}

bool ModifyStorage::addModifiesStm(int stm, string variable)
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

bool ModifyStorage::addModifiesProc(string procedure, string variable)
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

bool ModifyStorage::containsStmVarPair(pair<int, string> pair)
{
	return stmVarPairList.find(pair) != stmVarPairList.end();
}

bool ModifyStorage::containsProcVarPair(pair<string, string> pair)
{
	return procVarPairList.find(pair) != procVarPairList.end();
}

unordered_set<string> ModifyStorage::getVarModifiedByStm(int stm)
{
	if (stmToVarMap.find(stm) != stmToVarMap.end())
	{
		return stmToVarMap.at(stm);
	}
	return {};
}

unordered_set<string> ModifyStorage::getVarModifiedByProc(string proc)
{
	if (procToVarMap.find(proc) != procToVarMap.end())
	{
		return procToVarMap.at(proc);
	}
	return {};
}

unordered_set<int> ModifyStorage::getStmModifying(string variable)
{
	if (varToStmMap.find(variable) != varToStmMap.end())
	{
		return varToStmMap.at(variable);
	}
	return {};
}

unordered_set<string> ModifyStorage::getProcModifying(string variable)
{
	if (varToProcMap.find(variable) != varToProcMap.end())
	{
		varToProcMap.at(variable);
	}
	return {};
}

unordered_set<pair<int, string>, intStringhash> ModifyStorage::getStmVarPairs()
{
	return stmVarPairList;
}

unordered_set<pair<string, string>, strPairhash> ModifyStorage::getProcVarPairs()
{
	return procVarPairList;
}
