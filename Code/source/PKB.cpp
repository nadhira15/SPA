#pragma once

#include "PKB.h"

PKB::PKB()
{
}

void PKB::addProc(string name)
{
	procName = name;
}

void PKB::addStatement(int stmNo, stmType type)
{
	stmTypeList.assign(stmNo, type);

	switch (type)
	{
		case read:
			readStmList.emplace(stmNo);
			break;
		case print:
			printStmList.emplace(stmNo);
			break;
		case assign:
			assignStmList.emplace(stmNo);
			break;
		case whileStm:
			whileStmList.emplace(stmNo);
			break;
		case ifStm:
			ifStmList.emplace(stmNo);
			break;
		default:
			break;
	}
}

void PKB::addVariable(string name)
{
	varList.emplace(name);
}

void PKB::addConstant(int value)
{
	constList.emplace(value);
}

bool PKB::addFollow(int stm1, int stm2)
{
	if (stm2 <= stm1 || stm1 <= 0 || stm2 <= 0)
	{
		return false;
	}
	return fStore.addFollowPair(stm1, stm2);
}

bool PKB::setFollowers(int stm, unordered_set<int> stmList)
{
	return fStore.setAllFollowing(stm, stmList);
}

bool PKB::setStmFollowedBy(int stm, unordered_set<int> stmList)
{
	return fStore.setAllFollowedBy(stm, stmList);
}

bool PKB::addParent(int stm1, int stm2)
{
	if (stm2 <= stm1 || stm1 <= 0 || stm2 <= 0)
	{
		return false;
	}
	return pStore.addParent_Child(stm1, stm2);
}

bool PKB::setAncestors(int stm, unordered_set<int> stmList)
{
	return pStore.setAncestors(stm, stmList);
}

bool PKB::setDescendants(int stm, unordered_set<int> stmList)
{
	return pStore.setDescendants(stm, stmList);
}

bool PKB::addUses(int stm, string variable)
{
	if (variable == "")
	{
		return false;
	}
	return uStore.addUses(stm, variable);
}

bool PKB::addUses(string procedure, string variable)
{
	if (procedure == "" || variable == "")
	{
		return false;
	}
	return uStore.addUses(procedure, variable);
}

bool PKB::addModifies(int stm, string variable)
{
	if (variable == "")
	{
		return false;
	}
	return mStore.addModifies(stm, variable);
}

bool PKB::addModifies(string procedure, string variable)
{
	if (procedure == "" || variable == "")
	{
		return false;
	}
	return mStore.addModifies(procedure, variable);
}

bool PKB::addAssign(int stm, string variable, string expr)
{
	return patternList.emplace(stm, pair<string, string>(variable, expr)).second;
}

string PKB::getProcName()
{
	return procName;
}

int PKB::getTotalStmNo()
{
	return stmTypeList.size();
}

stmType PKB::getStmType(int stm)
{
	return stmTypeList.at(stm);
}

unordered_set<int> PKB::getReadStms()
{
	return readStmList;
}

unordered_set<int> PKB::getPrintStms()
{
	return printStmList;
}

unordered_set<int> PKB::getAssignStms()
{
	return assignStmList;
}

unordered_set<int> PKB::getIfStms()
{
	return ifStmList;
}

unordered_set<int> PKB::getWhileStms()
{
	return whileStmList;
}

unordered_set<string> PKB::getVariables()
{
	return varList;
}

unordered_set<int> PKB::getConstants()
{
	return constList;
}

bool PKB::hasFollowRelation()
{
	return !fStore.isEmpty();
}

bool PKB::hasFollow_S_Pair(int stm1, int stm2)
{
	return fStore.containsFSPair(pair<int, int>(stm1, stm2));
}

int PKB::getPrvStm(int stm)
{
	return fStore.getPrevOf(stm);
}

int PKB::getNxtStm(int stm)
{
	return fStore.getNextOf(stm);
}

unordered_set<int> PKB::getAllFollowing(int stm)
{
	return fStore.getAllFollowing(stm);
}

unordered_set<int> PKB::getAllFollowedBy(int stm)
{
	return fStore.getAllFollowedBy(stm);
}

unordered_set<int> PKB::getAllFollowers()
{
	return fStore.getFollowerList();
}

unordered_set<int> PKB::getAllFollowed()
{
	return fStore.getFollowedList;
}

unordered_set<pair<int, int>> PKB::getFollowPairs()
{
	return fStore.getFPairList();
}

unordered_set<pair<int, int>> PKB::getFollow_S_Pairs()
{
	return fStore.getF_S_PairList();
}

bool PKB::hasParentRelation()
{
	return !pStore.isEmpty();
}

bool PKB::isParent(int stm)
{
	return pStore.isParent(stm);
}

bool PKB::isChild(int stm)
{
	return pStore.isChild(stm);
}

bool PKB::hasAncDescPair(int stm1, int stm2)
{
	return pStore.containsAnc_Desc(pair<int, int>(stm1, stm2));
}

int PKB::getParent(int stm)
{
	return pStore.getParentOf(stm);
}

unordered_set<int> PKB::getChildren(int stm)
{
	return pStore.getChildrenOf(stm);
}

unordered_set<int> PKB::getAllAncestors(int stm)
{
	return pStore.getAncestorsOf(stm);
}

unordered_set<int> PKB::getAllDescendants(int stm)
{
	return pStore.getDescendantsOf(stm);
}

unordered_set<int> PKB::getAllParents()
{
	return pStore.getParentList;
}

unordered_set<int> PKB::getAllChildren()
{
	return pStore.getChildrenList();
}

unordered_set<pair<int, int>> PKB::getParentChildPairs()
{
	return pStore.getParent_ChildList();
}

unordered_set<pair<int, int>> PKB::getAncDescPairs()
{
	return pStore.getAnc_DescList();
}

bool PKB::isUsing(int stm, string variable)
{
	return uStore.containsStmVarPair(pair<int, string>(stm, variable) );
}

bool PKB::isUsing(string procedure, string variable)
{
	return uStore.containsProcVarPair(pair<string, string>(procedure, variable));
}

unordered_set<string> PKB::getUsedVar(int stm)
{
	return uStore.getVarUsedBy(stm);
}

unordered_set<string> PKB::getUsedVar(string procedure)
{
	return uStore.getVarUsedBy(procedure);
}

unordered_set<int> PKB::getStmUsing(string variable)
{
	return uStore.getStmUsing(variable);
}

unordered_set<string> PKB::getProcUsing(string variable)
{
	return uStore.getProcUsing(variable);
}

unordered_set<pair<int, string>> PKB::getStmVarUsePairs()
{
	return uStore.getStmVarPairs;
}

unordered_set<pair<string, string>> PKB::getProcVarUsePairs()
{
	return uStore.getProcVarPairs;
}

bool PKB::isModifying(int stm, string variable)
{
	return mStore.containsStmVarPair(pair<int, string>(stm, variable));
}

bool PKB::isModifying(string procedure, string variable)
{
	return mStore.containsProcVarPair(pair<string, string>(procedure, variable));
}

string PKB::getModifiedVar(int stm)
{
	return mStore.getVarModifiedBy(stm);
}

string PKB::getModifiedVar(string procedure)
{
	return mStore.getVarModifiedBy(procedure);
}

unordered_set<int> PKB::getStmModifying(string variable)
{
	return mStore.getStmModifying(variable);
}

unordered_set<string> PKB::getProcModifying(string variable)
{
	return mStore.getProcModifying(variable);
}

unordered_set<pair<int, string>> PKB::getStmVarModifyPairs()
{
	return mStore.getStmVarPairs;
}

unordered_set<pair<string, string>> PKB::getProcVarModifyPairs()
{
	return mStore.getProcVarPairs;
}

vector<int> PKB::findPattern(string variable, string expr, bool isExclusive)
{
	vector<int> validStm;
	for each (const auto elem in patternList)
	{
		if (elem.second.first.compare(variable) == 0)
		{
			if (isExclusive && elem.second.second.compare(expr) == 0)
			{
				validStm.push_back(elem.first);
			}
			else if (!isExclusive && elem.second.second.find(expr) != string::npos)
			{
				validStm.push_back(elem.first);
			}
		}
	}
	return validStm;
}

vector<int> PKB::findPattern(string expr, bool isExclusive)
{
	vector<int> validStm;
	for each (const auto elem in patternList)
	{
		if (isExclusive && elem.second.second.compare(expr) == 0)
		{
			validStm.push_back(elem.first);
		}
		else if (!isExclusive && elem.second.second.find(expr) != string::npos)
		{
			validStm.push_back(elem.first);
		}
	}
	return validStm;
}
