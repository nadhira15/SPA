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
	return fStore.addFollowPair(stm1, stm2);
}

bool PKB::addFollow_S(int stm1, int stm2)
{
	return fStore.addFollow_S_Pair(stm1, stm2);
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
	return fStore.isEmpty();
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
