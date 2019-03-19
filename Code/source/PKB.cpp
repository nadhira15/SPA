#include "PKB.h"

string PKB::procName;
vector<stmType> PKB::stmTypeList;
unordered_set<string> PKB::varList;
unordered_set<string> PKB::constList;
unordered_set<int> PKB::readStmList;
unordered_set<int> PKB::printStmList;
unordered_set<int> PKB::assignStmList;
unordered_set<int> PKB::ifStmList;
unordered_set<int> PKB::whileStmList;

FollowStorage PKB::fStore;
ParentStorage PKB::pStore;
UseStorage PKB::uStore;
ModifyStorage PKB::mStore;
unordered_map<int, pair<string, string> > PKB::patternList;

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

void PKB::addConstant(string value)
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

bool PKB::addUsesStm(int stm, string variable)
{
	if (variable == "")
	{
		return false;
	}
	return uStore.addUsesStm(stm, variable);
}

bool PKB::addUsesProc(string procedure, string variable)
{
	if (procedure == "" || variable == "")
	{
		return false;
	}
	return uStore.addUsesProc(procedure, variable);
}

bool PKB::addModifiesStm(int stm, string variable)
{
	if (variable == "")
	{
		return false;
	}
	return mStore.addModifiesStm(stm, variable);
}

bool PKB::addModifiesProc(string procedure, string variable)
{
	if (procedure == "" || variable == "")
	{
		return false;
	}
	return mStore.addModifiesProc(procedure, variable);
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

unordered_set<string> PKB::getConstants()
{
	return constList;
}

bool PKB::hasFollowRelation()
{
	return !fStore.isEmpty();
}

bool PKB::hasFollowStarPair(int stm1, int stm2)
{
	return fStore.hasFollowStarPair(pair<int, int>(stm1, stm2));
}

int PKB::getStmFollowedBy(int stm)
{
	return fStore.getStmFollowedBy(stm);
}

int PKB::getFollower(int stm)
{
	return fStore.getFollower(stm);
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
	return fStore.getAllFollowers();
}

unordered_set<int> PKB::getAllFollowed()
{
	return fStore.getAllFollowed();
}

unordered_set<pair<int, int>, intPairhash> PKB::getFollowPairs()
{
	return fStore.getFollowPairs();
}

unordered_set<pair<int, int>, intPairhash> PKB::getFollowStarPairs()
{
	return fStore.getFollowStarPairs();
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
	return pStore.hasAncDescPair(pair<int, int>(stm1, stm2));
}

int PKB::getParent(int stm)
{
	return pStore.getParent(stm);
}

unordered_set<int> PKB::getChildren(int stm)
{
	return pStore.getChildren(stm);
}

unordered_set<int> PKB::getAncestors(int stm)
{
	return pStore.getAncestors(stm);
}

unordered_set<int> PKB::getDescendants(int stm)
{
	return pStore.getDescendants(stm);
}

unordered_set<int> PKB::getAllParents()
{
	return pStore.getAllParent();
}

unordered_set<int> PKB::getAllChildren()
{
	return pStore.getAllChildren();
}

unordered_set<pair<int, int>, intPairhash> PKB::getParentChildPairs()
{
	return pStore.getParentChildPairs();
}

unordered_set<pair<int, int>, intPairhash> PKB::getAncDescPairs()
{
	return pStore.getAncDescPair();
}

bool PKB::isUsing(int stm, string variable)
{
	return uStore.containsStmVarPair(pair<int, string>(stm, variable) );
}

bool PKB::isUsing(string procedure, string variable)
{
	return uStore.containsProcVarPair(pair<string, string>(procedure, variable));
}

unordered_set<string> PKB::getVarUsedByStm(int stm)
{
	return uStore.getVarUsedByStm(stm);
}

unordered_set<string> PKB::getVarUsedByProc(string procedure)
{
	return uStore.getVarUsedByProc(procedure);
}

unordered_set<int> PKB::getStmUsing(string variable)
{
	return uStore.getStmUsing(variable);
}

unordered_set<string> PKB::getProcUsing(string variable)
{
	return uStore.getProcUsing(variable);
}

unordered_set<pair<int, string>, intStringhash> PKB::getStmVarUsePairs()
{
	return uStore.getStmVarPairs();
}

unordered_set<pair<string, string>, strPairhash> PKB::getProcVarUsePairs()
{
	return uStore.getProcVarPairs();
}

bool PKB::isModifying(int stm, string variable)
{
	return mStore.containsStmVarPair(pair<int, string>(stm, variable));
}

bool PKB::isModifying(string procedure, string variable)
{
	return mStore.containsProcVarPair(pair<string, string>(procedure, variable));
}

unordered_set<string> PKB::getModifiedVar(int stm)
{
	return mStore.getVarModifiedByStm(stm);
}

unordered_set<string> PKB::getModifiedVar(string procedure)
{
	return mStore.getVarModifiedByProc(procedure);
}

unordered_set<int> PKB::getStmModifying(string variable)
{
	return mStore.getStmModifying(variable);
}

unordered_set<string> PKB::getProcModifying(string variable)
{
	return mStore.getProcModifying(variable);
}

unordered_set<pair<int, string>, intStringhash> PKB::getStmVarModifyPairs()
{
	return mStore.getStmVarPairs();
}

unordered_set<pair<string, string>, strPairhash> PKB::getProcVarModifyPairs()
{
	return mStore.getProcVarPairs();
}

/*
	Search for assign statements with pattern matching
		- the specified variable ont the left side and
		- the specified expression on the right side
	If isExclusive is true, the function finds for an exact match to the specfied expression.
	If false, it finds for a matching substring instead.
*/
unordered_set<int> PKB::findPattern(string variable, string expr, bool isExclusive)
{
	unordered_set<int> validStm;
	for each (const auto elem in patternList)
	{
		if (elem.second.first.compare(variable) == 0)
		{
			if (isExclusive && elem.second.second.compare(expr) == 0)
			{
				validStm.emplace(elem.first);
			}
			else if (!isExclusive && elem.second.second.find(expr) != string::npos)
			{
				validStm.emplace(elem.first);
			}
		}
	}
	return validStm;
}

/*
	Search for assign statements with pattern matching
		- the specified expression on the right side
	If isExclusive is true, the function finds for an exact match to the specfied expression.
	If false, it finds for a matching substring instead.
*/
unordered_set<int> PKB::findPattern(string expr, bool isExclusive)
{
	unordered_set<int> validStm;
	for each (const auto elem in patternList)
	{
		if (isExclusive && elem.second.second.compare(expr) == 0)
		{
			validStm.emplace(elem.first);
		}
		else if (!isExclusive && elem.second.second.find(expr) != string::npos)
		{
			validStm.emplace(elem.first);
		}
	}
	return validStm;
}

/*
	Search for pairs of an assign statement and left-hand side variable with
		the right side expression matching the specified expression
	If isExclusive is true, the function finds for an exact match to the specfied expression.
	If false, it finds for a matching substring instead.
*/
unordered_set<pair<int, string>, intStringhash> PKB::findPatternPairs(string expr, bool isExclusive)
{
	unordered_set<pair<int, string>, intStringhash> validPairs;
	for each (const auto elem in patternList)
	{
		if (isExclusive && elem.second.second.compare(expr) == 0)
		{
			validPairs.emplace(pair<int, string>(elem.first, elem.second.first));
		}
		else if (!isExclusive && elem.second.second.find(expr) != string::npos)
		{
			validPairs.emplace(pair<int, string>(elem.first, elem.second.first));
		}
	}
	return validPairs;
}
