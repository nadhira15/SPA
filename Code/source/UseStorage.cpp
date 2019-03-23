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

/*
	add the Uses relation for a statement into the relevant lists and maps in the storage
	Returns false if the pair is already exist
*/
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

/*
	add the Uses relation for a procedure into the relevant lists and maps in the storage
	Returns false if the pair is already exist
*/
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

// returns true if the specified <statement, variable> pair is found
bool UseStorage::containsStmVarPair(pair<int, string> pair)
{
	return stmVarPairList.find(pair) != stmVarPairList.end();
}

// returns true if the specified <procedure, variable> pair is found
bool UseStorage::containsProcVarPair(pair<string, string> pair)
{
	return procVarPairList.find(pair) != procVarPairList.end();
}

/*
	return the list of variables that is used by 'stm'
	return an empty set if 'stm' is not found
*/
unordered_set<string> UseStorage::getVarUsedByStm(int stm)
{
	if (stmToVarMap.find(stm) != stmToVarMap.end())
	{
		return stmToVarMap.at(stm);
	}
	return {};
}

/*
	return the list of variables that is used by 'procedure'
	return an empty set if 'procedure' is not found
*/
unordered_set<string> UseStorage::getVarUsedByProc(string procedure)
{
	if (procToVarMap.find(procedure) != procToVarMap.end())
	{
		return procToVarMap.at(procedure);
	}
	return {};
}

/*
	return the list of statements that is using 'variable'
	return an empty set if 'variable' is not found
*/
unordered_set<int> UseStorage::getStmUsing(string variable)
{
	if (varToStmMap.find(variable) != varToStmMap.end())
	{
		return varToStmMap.at(variable);
	}
	return {};
}

/*
	return the list of procedures that is using 'variable'
	return an empty set if 'variable' is not found
*/
unordered_set<string> UseStorage::getProcUsing(string variable)
{
	if (varToProcMap.find(variable) != varToProcMap.end())
	{
		return varToProcMap.at(variable);
	}
	return {};
}

// returns a list of all Uses pairs for statements
unordered_set<pair<int, string>, intStringhash> UseStorage::getStmVarPairs()
{
	return stmVarPairList;
}

// returns a list of all Uses pairs for procedures
unordered_set<pair<string, string>, strPairhash> UseStorage::getProcVarPairs()
{
	return procVarPairList;
}
