#include "UseStorage.h"

UseStorage::UseStorage()
{
}

bool UseStorage::addUses(int stm, string variable)
{
	if (!stmVarPairs.emplace(pair<int, string>(stm, variable)).second)
	{
		return false;
	}
	varLists_Stm.emplace(stm, unordered_set<string>{ variable } );
	stmLists.emplace(variable, unordered_set<int>{ stm } );
	stmLists.emplace("", unordered_set<int>{ stm } );
	return true;
}

bool UseStorage::addUses(string procedure, string variable)
{
	if (!procVarPairs.emplace(pair<string, string>(procedure, variable)).second)
	{
		return false;
	}
	varLists_Proc.emplace(procedure, unordered_set<string>{ variable });
	procLists.emplace(variable, unordered_set<string>{ procedure });
	procLists.emplace("", unordered_set<string>{ procedure });
	return true;
}

bool UseStorage::containsStmVarPair(pair<int, string> pair)
{
	return stmVarPairs.find(pair) != stmVarPairs.end();
}

bool UseStorage::containsProcVarPair(pair<string, string> pair)
{
	return procVarPairs.find(pair) != procVarPairs.end();
}

unordered_set<string> UseStorage::getVarUsedBy(int stm)
{
	return varLists_Stm.find(stm)->second;
}

unordered_set<string> UseStorage::getVarUsedBy(string proc)
{
	return varLists_Proc.find(proc)->second;
}

unordered_set<int> UseStorage::getStmUsing(string variable)
{
	return stmLists.find(variable)->second;
}

unordered_set<string> UseStorage::getProcUsing(string variable)
{
	return procLists.find(variable)->second;
}

unordered_set<pair<int, string>> UseStorage::getStmVarPairs()
{
	return stmVarPairs;
}

unordered_set<pair<string, string>> UseStorage::getProcVarPairs()
{
	return procVarPairs;
}
