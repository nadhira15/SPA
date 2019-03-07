#include "UseStorage.h"

unordered_set<pair<int, string>, intStringhash> UseStorage::stmVarPairs;
unordered_set<pair<string, string>, strPairhash> UseStorage::procVarPairs;
unordered_map<int, unordered_set<string> > UseStorage::varLists_Stm;
unordered_map<string, unordered_set<string> > UseStorage::varLists_Proc;
unordered_map<string, unordered_set<int> > UseStorage::stmLists;
unordered_map<string, unordered_set<string> > UseStorage::procLists;

UseStorage::UseStorage()
{
}

bool UseStorage::addUses(int stm, string variable)
{
	if (!stmVarPairs.emplace(pair<int, string>(stm, variable)).second)
	{
		return false;
	}
	if (!varLists_Stm.emplace(stm, unordered_set<string>{ variable }).second)
	{
		varLists_Stm.at(stm).emplace(variable);
	}
	if (!stmLists.emplace(variable, unordered_set<int>{ stm }).second)
	{
		stmLists.at(variable).emplace(stm);
	}
	if (!stmLists.emplace("", unordered_set<int>{ stm }).second)
	{
		stmLists.at("").emplace(stm);
	}
	return true;
}

bool UseStorage::addUses(string procedure, string variable)
{
	if (!procVarPairs.emplace(pair<string, string>(procedure, variable)).second)
	{
		return false;
	}
	if (!varLists_Proc.emplace(procedure, unordered_set<string>{ variable }).second)
	{
		varLists_Proc.at(procedure).emplace(variable);
	}
	if (!procLists.emplace(variable, unordered_set<string>{ procedure }).second)
	{
		procLists.at(variable).emplace(procedure);
	}
	if (!procLists.emplace("", unordered_set<string>{ procedure }).second)
	{
		procLists.at("").emplace(procedure);
	}
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
	if (varLists_Stm.find(stm) != varLists_Stm.end())
	{
		return varLists_Stm.at(stm);
	}
	return {};
}

unordered_set<string> UseStorage::getVarUsedBy(string proc)
{
	if (varLists_Proc.find(proc) != varLists_Proc.end())
	{
		return varLists_Proc.at(proc);
	}
	return {};
}

unordered_set<int> UseStorage::getStmUsing(string variable)
{
	if (stmLists.find(variable) != stmLists.end())
	{
		return stmLists.at(variable);
	}
	return {};
}

unordered_set<string> UseStorage::getProcUsing(string variable)
{
	if (procLists.find(variable) != procLists.end())
	{
		return procLists.at(variable);
	}
	return {};
}

unordered_set<pair<int, string>, intStringhash> UseStorage::getStmVarPairs()
{
	return stmVarPairs;
}

unordered_set<pair<string, string>, strPairhash> UseStorage::getProcVarPairs()
{
	return procVarPairs;
}
