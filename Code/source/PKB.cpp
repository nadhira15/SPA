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
	if (stm2 <= stm1)
	{
		return false;
	}
	return fStore.addFollowPair(stm1, stm2);
}

bool PKB::addFollow_S(int stm1, int stm2)
{
	if (stm2 <= stm1)
	{
		return false;
	}
	return fStore.addFollow_S_Pair(stm1, stm2);
}

bool PKB::setAllFollowing(int stm, unordered_set<int> stmList)
{
	return fStore.setAllFollowing(stm, stmList);
}

bool PKB::setAllFollowBy(int stm, unordered_set<int> stmList)
{
	return fStore.setAllFollowedBy(stm, stmList);
}

bool PKB::addParent(int stm1, int stm2)
{
	if (stm2 <= stm1)
	{
		return false;
	}
	return pStore.addParent_Child(stm1, stm2);
}

bool PKB::addParent_S(int stm1, int stm2)
{
	if (stm2 <= stm1)
	{
		return false;
	}
	return pStore.addAnc_Desc(stm1, stm2);
}

bool PKB::setAllAncestors(int stm, unordered_set<int> stmList)
{
	return pStore.setAncestors(stm, stmList);
}

bool PKB::setAllDescendants(int stm, unordered_set<int> stmList)
{
	return pStore.setDescendants(stm, stmList);
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
