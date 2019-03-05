#include "ModifyStorage.h"

unordered_set<pair<int, string>, intStringhash> ModifyStorage::stmVarPairs;
unordered_set<pair<string, string>, strPairhash> ModifyStorage::procVarPairs;
unordered_map<int, string> ModifyStorage::varList_Stm;
unordered_map<string, string> ModifyStorage::varList_Proc;
unordered_map<string, unordered_set<int> > ModifyStorage::stmLists;
unordered_map<string, unordered_set<string> > ModifyStorage::procLists;

ModifyStorage::ModifyStorage()
{
}

bool ModifyStorage::addModifies(int stm, string variable)
{
	if (!stmVarPairs.emplace(pair<int, string>(stm, variable)).second)
	{
		return false;
	}
	varList_Stm.emplace(stm, variable);
	stmLists.emplace(variable, unordered_set<int>{ stm });
	stmLists.emplace("", unordered_set<int>{ stm });
	return true;
}

bool ModifyStorage::addModifies(string procedure, string variable)
{
	if (!procVarPairs.emplace(pair<string, string>(procedure, variable)).second)
	{
		return false;
	}
	varList_Proc.emplace(procedure, variable);
	procLists.emplace(variable, unordered_set<string>{ procedure });
	procLists.emplace("", unordered_set<string>{ procedure });
	return true;
}

bool ModifyStorage::containsStmVarPair(pair<int, string> pair)
{
	return stmVarPairs.find(pair) != stmVarPairs.end();
}

bool ModifyStorage::containsProcVarPair(pair<string, string> pair)
{
	return procVarPairs.find(pair) != procVarPairs.end();
}

string ModifyStorage::getVarModifiedBy(int stm)
{
	if (varList_Stm.find(stm) != varList_Stm.end())
	{
		return varList_Stm.at(stm);
	}
	return "";
}

string ModifyStorage::getVarModifiedBy(string proc)
{
	if (varList_Proc.find(proc) != varList_Proc.end())
	{
		return varList_Proc.at(proc);
	}
	return "";
}

unordered_set<int> ModifyStorage::getStmModifying(string variable)
{
	if (stmLists.find(variable) != stmLists.end())
	{
		return stmLists.at(variable);
	}
	return {};
}

unordered_set<string> ModifyStorage::getProcModifying(string variable)
{
	if (procLists.find(variable) != procLists.end())
	{
		procLists.at(variable);
	}
	return {};
}

unordered_set<pair<int, string>, intStringhash> ModifyStorage::getStmVarPairs()
{
	return stmVarPairs;
}

unordered_set<pair<string, string>, strPairhash> ModifyStorage::getProcVarPairs()
{
	return procVarPairs;
}
