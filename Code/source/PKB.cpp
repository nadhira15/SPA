#include "PKB.h"
#include"RunTimeDesignExtractor.h"

unordered_set<string> PKB::procList;
unordered_map<string, vector<int>> PKB::procStmList;
vector<stmType> PKB::stmTypeList;
unordered_set<string> PKB::varList;
unordered_set<string> PKB::constList;
unordered_set<int> PKB::readStmList;
unordered_set<int> PKB::printStmList;
unordered_set<int> PKB::assignStmList;
unordered_set<int> PKB::ifStmList;
unordered_set<int> PKB::whileStmList;
unordered_set<int> PKB::callStmList;

FollowStorage PKB::fStore;
ParentStorage PKB::pStore;
UseStorage PKB::uStore;
ModifyStorage PKB::mStore;
CallStorage PKB::cStore;
NextStorage PKB::nStore;
ControlVariableStorage PKB::cvStore;
unordered_map<int, pair<string, string> > PKB::patternList;

PKB::PKB()
{
}

bool PKB::addProc(string name)
{
	return procList.emplace(name).second;
}

void PKB::addStatement(int stmNo, stmType type)
{
	stmTypeList.push_back(type);
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
		case call:
			callStmList.emplace(stmNo);
			break;
		default:
			break;
	}
}

void PKB::addStatement(int stmNo, stmType type, string procedure)
{
	stmTypeList.push_back(type);
	if (!procStmList.emplace(procedure, vector<int>{stmNo}).second)
	{
		procStmList.find(procedure)->second.push_back(stmNo);
	}

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
		case call:
			callStmList.emplace(stmNo);
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
	return fStore.addFollow(stm1, stm2);
}

bool PKB::setAllFollowing(int stm, unordered_set<int> stmList)
{
	return fStore.setAllFollowing(stm, stmList);
}

bool PKB::setAllFollowedBy(int stm, unordered_set<int> stmList)
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
	if (stm <= 0 || variable == "")
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

bool PKB::addCall(string proc1, string proc2)
{
	if (proc1 == "" || proc2 == "")
	{
		return false;
	}
	return cStore.addCall(proc1, proc2);
}

bool PKB::addCall(string proc1, string proc2, int stmNo)
{
	if (proc1 == "" || proc2 == "" || stmNo <= 0)
	{
		return false;
	}
	return cStore.addCall(proc1, proc2, stmNo);
}

bool PKB::setCallAnc(string proc, unordered_set<string> procList)
{
	return cStore.setCallAnc(proc, procList);
}

bool PKB::setCallDesc(string proc, unordered_set<string> procList)
{
	return cStore.setCallDesc(proc, procList);
}

bool PKB::addNext(int line1, int line2)
{
	if (line1 <= 0 || line2 <= 0)
	{
		return false;
	}
	return nStore.addNext(line1, line2);
}

bool PKB::addAssignPattern(int stm, string variable, string expr)
{
	return patternList.emplace(stm, pair<string, string>(variable, expr)).second;
}

unordered_set<string> PKB::getProcList()
{
	return procList;
}

vector<int> PKB::getStmList(string procedure)
{
	if (procStmList.find(procedure) != procStmList.end())
	{
		return procStmList.at(procedure);
	}
	return {};
}

int PKB::getTotalStmNo()
{
	return stmTypeList.size();
}

stmType PKB::getStmType(int stm)
{
	return stmTypeList.at(stm - 1);
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

unordered_set<int> PKB::getCallStms()
{
	return callStmList;
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

bool PKB::isStmUsing(int stm, string variable)
{
	return uStore.containsStmVarPair(pair<int, string>(stm, variable) );
}

bool PKB::isProcUsing(string procedure, string variable)
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

bool PKB::isStmModifying(int stm, string variable)
{
	return mStore.containsStmVarPair(pair<int, string>(stm, variable));
}

bool PKB::isProcModifying(string procedure, string variable)
{
	return mStore.containsProcVarPair(pair<string, string>(procedure, variable));
}

unordered_set<string> PKB::getVarModifiedByStm(int stm)
{
	return mStore.getVarModifiedByStm(stm);
}

unordered_set<string> PKB::getVarModifiedByProc(string procedure)
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

bool PKB::hasCallRelation()
{
	return !cStore.isEmpty();
}

bool PKB::isCaller(string procedure)
{
	return cStore.isCaller(procedure);
}

bool PKB::isCallee(string procedure)
{
	return cStore.isCallee(procedure);
}

bool PKB::hasCallStarPair(string proc1, string proc2)
{
	return cStore.hasCallStarPair(pair<string, string>(proc1, proc2));
}

unordered_set<string> PKB::getCaller(string procedure)
{
	return cStore.getCaller(procedure);
}

unordered_set<string> PKB::getCallee(string procedure)
{
	return cStore.getCallee(procedure);
}

unordered_set<string> PKB::getCallAnc(string procedure)
{
	return cStore.getCallAnc(procedure);
}

unordered_set<string> PKB::getCallDesc(string procedure)
{
	return cStore.getCallDesc(procedure);
}

unordered_set<string> PKB::getAllCallers()
{
	return cStore.getAllCallers();
}

unordered_set<string> PKB::getAllCallees()
{
	return cStore.getAllCallees();
}

unordered_set<pair<string, string>, strPairhash> PKB::getCallPairs()
{
	return cStore.getCallPairs();
}

unordered_set<pair<string, string>, strPairhash> PKB::getCallStarPairs()
{
	return cStore.getCallStarPairs();
}

string PKB::getProcCalledBy(int stm)
{
	return cStore.getProcCalledBy(stm);
}

unordered_set<int> PKB::getStmCalling(string procedure)
{
	return cStore.getStmCalling(procedure);
}

bool PKB::hasNextRelation()
{
	return !nStore.isEmpty();
}

bool PKB::hasNextStarPair(int line1, int line2)
{
	// call DE
	return RunTimeDesignExtractor::extractNextStarPair(line1, line2);
}

unordered_set<int> PKB::getNext(int line)
{
	return nStore.getNext(line);
}

unordered_set<int> PKB::getPrev(int line)
{
	return nStore.getPrev(line);
}

unordered_set<int> PKB::getAllLnAfter(int line)
{
	// call DE
	return {};
}

unordered_set<int> PKB::getAllLnBefore(int line)
{
	// call DE
	return {};
}

unordered_set<int> PKB::getAllNext()
{
	return nStore.getAllNext();
}

unordered_set<int> PKB::getAllPrev()
{
	return nStore.getAllPrev();
}

unordered_set<pair<int, int>, intPairhash> PKB::getNextPairs()
{
	return nStore.getNextPairs();
}

unordered_set<pair<int, int>, intPairhash> PKB::getNextStarPairs()
{
	// call DE
	return {};
}

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

void PKB::addIfControlVariable(int stm, string variable)
{
	cvStore.addIfControlVariable(stm, variable);
}

void PKB::addWhileControlVariable(int stm, string variable)
{
	cvStore.addWhileControlVariable(stm, variable);
}

std::unordered_set<int> PKB::getAllIfWithControls()
{
	return cvStore.getAllIfWithControls();
}

std::unordered_set<int> PKB::getAllWhileWithControls()
{
	return cvStore.getAllWhileWithControls();
}

std::unordered_set<int> PKB::getIfStmWithControlVariable(std::string variable)
{
	return cvStore.getIfStmWithControlVariable(variable);
}

std::unordered_set<int> PKB::getWhileStmWithControlVariable(std::string variable)
{
	return cvStore.getWhileStmWithControlVariable(variable);
}

std::unordered_set<std::pair<int, std::string>, intStringhash> PKB::getIfStmControlVariablePair()
{
	return cvStore.getIfStmControlVariablePair();
}

std::unordered_set<std::pair<int, std::string>, intStringhash> PKB::getWhileStmControlVariablePair()
{
	return cvStore.getWhileStmControlVariablePair()
}

void PKB::erase()
{
	procList.erase(procList.begin(), procList.end());
	procStmList.erase(procStmList.begin(), procStmList.end());
	stmTypeList.erase(stmTypeList.begin(), stmTypeList.end());
	varList.erase(varList.begin(), varList.end());
	constList.erase(constList.begin(), constList.end());
	readStmList.erase(readStmList.begin(), readStmList.end());
	printStmList.erase(printStmList.begin(), printStmList.end());
	assignStmList.erase(assignStmList.begin(), assignStmList.end());
	ifStmList.erase(ifStmList.begin(), ifStmList.end());
	whileStmList.erase(whileStmList.begin(), whileStmList.end());
	callStmList.erase(callStmList.begin(), callStmList.end());
	patternList.erase(patternList.begin(), patternList.end());
	fStore.erase();
	pStore.erase();
	uStore.erase();
	mStore.erase();
	cStore.erase();
	nStore.erase();
}