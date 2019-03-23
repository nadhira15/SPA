#include "PKB.h"

unordered_set<string> PKB::procList;
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

// add a procedure to procList
void PKB::addProc(string name)
{
	procList.emplace(name);
}

//add statement to its respective StmList and set stmTypeList[stmNo] to type 
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

// add variable to varList
void PKB::addVariable(string name)
{
	varList.emplace(name);
}

// add constant to constList
void PKB::addConstant(string value)
{
	constList.emplace(value);
}

/*
	add the follows relation in FollowStorage
	Returns false if	1) the pair is already stored
						2) the followed statement has another follower stored
						3) the follower is following another statement
						4) stm2 <= stm1 or stm1, stm2 <= 0
*/
bool PKB::addFollow(int stm1, int stm2)
{
	if (stm2 <= stm1 || stm1 <= 0 || stm2 <= 0)
	{
		return false;
	}
	return fStore.addFollow(stm1, stm2);
}

/*
	Sets the list of followers of 'stm' to be 'stmList' in FollowStorage
	Every FollowStar pair is stored as well
	If stm already has a list of followers, it is not replaced and it return false
*/
bool PKB::setAllFollowing(int stm, unordered_set<int> stmList)
{
	return fStore.setAllFollowing(stm, stmList);
}

/*
	Sets the list of followed of 'stm' to be 'stmList' in FollowStorage
	Every FollowStar pair is stored as well
	If stm already has a list of followed, it is not replaced and it return false
*/
bool PKB::setAllFollowedBy(int stm, unordered_set<int> stmList)
{
	return fStore.setAllFollowedBy(stm, stmList);
}


/*
	Adds the parent relation into the various lists in the storage
	Returns false if	1) the pair is already stored
						2) the child already has another parent
						3) stm2 <= stm1 or stm1, stm2 <= 0
*/
bool PKB::addParent(int stm1, int stm2)
{
	if (stm2 <= stm1 || stm1 <= 0 || stm2 <= 0)
	{
		return false;
	}
	return pStore.addParent_Child(stm1, stm2);
}

/*
	Sets the list of ancestors of 'stm' in ParentStorage
	Each Parent* pair is stored as well
	If stm already has a list of ancestors, it is not replaced and it return false
*/
bool PKB::setAncestors(int stm, unordered_set<int> stmList)
{
	return pStore.setAncestors(stm, stmList);
}

/*
	Sets the list of descendants of 'stm' in ParentStorage
	Each Parent* pair is stored as well
	If stm already has a list of descendants, it is not replaced and it return false
*/
bool PKB::setDescendants(int stm, unordered_set<int> stmList)
{
	return pStore.setDescendants(stm, stmList);
}

/*
	add the Uses relation for a Statement in UseStorage
	Returns false if	1) the pair is already stored
						2) stm <= 0 and variable is an empty string
*/
bool PKB::addUsesStm(int stm, string variable)
{
	if (stm <= 0 || variable == "")
	{
		return false;
	}
	return uStore.addUsesStm(stm, variable);
}

/*
	add the Uses relation for a Procedure in UseStorage
	Returns false if	1) the pair is already stored
						2) procedure or variable is an empty string
*/
bool PKB::addUsesProc(string procedure, string variable)
{
	if (procedure == "" || variable == "")
	{
		return false;
	}
	return uStore.addUsesProc(procedure, variable);
}

/*
	add the Modifies relation for a Statement in UseStorage
	Returns false if	1) the pair is already stored
						2) stm <= 0 and variable is an empty string
*/
bool PKB::addModifiesStm(int stm, string variable)
{
	if (variable == "")
	{
		return false;
	}
	return mStore.addModifiesStm(stm, variable);
}

/*
	add the Modifies relation for a Procedure in UseStorage
	Returns false if	1) the pair is already stored
						2) procedure or variable is an empty string
*/
bool PKB::addModifiesProc(string procedure, string variable)
{
	if (procedure == "" || variable == "")
	{
		return false;
	}
	return mStore.addModifiesProc(procedure, variable);
}

/* 
	add a pattern for an assign statement to patternList
	returns false if 'stm' already exist in the list with another pattern
*/
bool PKB::addAssignPattern(int stm, string variable, string expr)
{
	return patternList.emplace(stm, pair<string, string>(variable, expr)).second;
}

// returns the stored list of procedures
unordered_set<string> PKB::getProcList()
{
	return procList;
}

// returns the total number of statements in the entire program
int PKB::getTotalStmNo()
{
	return stmTypeList.size();
}

// return the statement type of stm
stmType PKB::getStmType(int stm)
{
	return stmTypeList.at(stm);
}

// returns the stored list of read statements
unordered_set<int> PKB::getReadStms()
{
	return readStmList;
}

// returns the stored list of print statements
unordered_set<int> PKB::getPrintStms()
{
	return printStmList;
}

// returns the stored list of assign statements
unordered_set<int> PKB::getAssignStms()
{
	return assignStmList;
}

// returns the stored list of if statements
unordered_set<int> PKB::getIfStms()
{
	return ifStmList;
}

// returns the stored list of while statements
unordered_set<int> PKB::getWhileStms()
{
	return whileStmList;
}

// returns the stored list of variables
unordered_set<string> PKB::getVariables()
{
	return varList;
}

// returns the stored list of constants
unordered_set<string> PKB::getConstants()
{
	return constList;
}

// checks if there is at least one follows relation
bool PKB::hasFollowRelation()
{
	return !fStore.isEmpty();
}

// checks if the relation Follows*(stm1, stm2) exist
bool PKB::hasFollowStarPair(int stm1, int stm2)
{
	return fStore.hasFollowStarPair(pair<int, int>(stm1, stm2));
}

/*
	return the statement followed by 'stm'
	return 0 if 'stm' is not found
*/
int PKB::getStmFollowedBy(int stm)
{
	return fStore.getStmFollowedBy(stm);
}

/*
	return the statement following 'stm'
	return 0 if 'stm' is not found
*/
int PKB::getFollower(int stm)
{
	return fStore.getFollower(stm);
}

/*
	return a list of statements that is directly/indirectly following 'stm'
	return an empty set if 'stm' is not found
*/
unordered_set<int> PKB::getAllFollowing(int stm)
{
	return fStore.getAllFollowing(stm);
}

/*
	return a list of statements that is directly/indirectly followed by 'stm'
	return an empty set if 'stm' is not found
*/
unordered_set<int> PKB::getAllFollowedBy(int stm)
{
	return fStore.getAllFollowedBy(stm);
}

// returns a list of all statements that follows another
unordered_set<int> PKB::getAllFollowers()
{
	return fStore.getAllFollowers();
}

// returns a list of all statements that is followed by another
unordered_set<int> PKB::getAllFollowed()
{
	return fStore.getAllFollowed();
}

// returns a list of all follows pairs
unordered_set<pair<int, int>, intPairhash> PKB::getFollowPairs()
{
	return fStore.getFollowPairs();
}

// returns a list of all follows* pairs
unordered_set<pair<int, int>, intPairhash> PKB::getFollowStarPairs()
{
	return fStore.getFollowStarPairs();
}

// checks if there is at least one Parent relationship
bool PKB::hasParentRelation()
{
	return !pStore.isEmpty();
}

// checks if stm is a parent of another
bool PKB::isParent(int stm)
{
	return pStore.isParent(stm);
}

// checks if stm is a child of another
bool PKB::isChild(int stm)
{
	return pStore.isChild(stm);
}

// checks if the relation Parent*(stm1, stm2) exist
bool PKB::hasAncDescPair(int stm1, int stm2)
{
	return pStore.hasAncDescPair(pair<int, int>(stm1, stm2));
}

/*
	return the statement that is the parent of 'stm'
	return 0 if 'stm' is not found
*/
int PKB::getParent(int stm)
{
	return pStore.getParent(stm);
}

/*
	return the list of statements that is the children of 'stm'
	return an empty set if 'stm' is not found
*/
unordered_set<int> PKB::getChildren(int stm)
{
	return pStore.getChildren(stm);
}

/*
	return the list of statements that is the ancestors of 'stm'
	return an empty set if 'stm' is not found
*/
unordered_set<int> PKB::getAncestors(int stm)
{
	return pStore.getAncestors(stm);
}

/*
	return the list of statements that is the descendants of 'stm'
	return an empty set if 'stm' is not found
*/
unordered_set<int> PKB::getDescendants(int stm)
{
	return pStore.getDescendants(stm);
}

// returns a list of all statements that is the parent of another
unordered_set<int> PKB::getAllParents()
{
	return pStore.getAllParent();
}

// returns a list of all statements that is the child of another
unordered_set<int> PKB::getAllChildren()
{
	return pStore.getAllChildren();
}

// returns a list of all parent pairs
unordered_set<pair<int, int>, intPairhash> PKB::getParentChildPairs()
{
	return pStore.getParentChildPairs();
}

// returns a list of all parent* pairs
unordered_set<pair<int, int>, intPairhash> PKB::getAncDescPairs()
{
	return pStore.getAncDescPair();
}

// checks if the relation Uses(stm, variable) exist
bool PKB::isStmUsing(int stm, string variable)
{
	return uStore.containsStmVarPair(pair<int, string>(stm, variable) );
}

// checks if the relation Uses(procedure, variable) exist
bool PKB::isProcUsing(string procedure, string variable)
{
	return uStore.containsProcVarPair(pair<string, string>(procedure, variable));
}

/*
	return the list of variables that is used by 'stm'
	return an empty set if 'stm' is not found
*/
unordered_set<string> PKB::getVarUsedByStm(int stm)
{
	return uStore.getVarUsedByStm(stm);
}

/*
	return the list of variables that is used by 'procedure'
	return an empty set if 'procedure' is not found
*/
unordered_set<string> PKB::getVarUsedByProc(string procedure)
{
	return uStore.getVarUsedByProc(procedure);
}

/*
	return the list of statements that is using 'variable'
	return an empty set if 'variable' is not found
*/
unordered_set<int> PKB::getStmUsing(string variable)
{
	return uStore.getStmUsing(variable);
}

/*
	return the list of procedure that is using 'variable'
	return an empty set if 'variable' is not found
*/
unordered_set<string> PKB::getProcUsing(string variable)
{
	return uStore.getProcUsing(variable);
}

// returns a list of all Uses pairs for statements
unordered_set<pair<int, string>, intStringhash> PKB::getStmVarUsePairs()
{
	return uStore.getStmVarPairs();
}

// returns a list of all Uses pairs for procedures
unordered_set<pair<string, string>, strPairhash> PKB::getProcVarUsePairs()
{
	return uStore.getProcVarPairs();
}

// checks if the relation Modifies(stm, variable) exist
bool PKB::isStmModifying(int stm, string variable)
{
	return mStore.containsStmVarPair(pair<int, string>(stm, variable));
}

// checks if the relation Modifies(procedure, variable) exist
bool PKB::isProcModifying(string procedure, string variable)
{
	return mStore.containsProcVarPair(pair<string, string>(procedure, variable));
}

/*
	return the list of variables that is modified by 'stm'
	return an empty set if 'stm' is not found
*/
unordered_set<string> PKB::getVarModifiedByStm(int stm)
{
	return mStore.getVarModifiedByStm(stm);
}

/*
	return the list of variables that is modified by 'procedure'
	return an empty set if 'procedure' is not found
*/
unordered_set<string> PKB::getVarModifiedByProc(string procedure)
{
	return mStore.getVarModifiedByProc(procedure);
}

/*
	return the list of statements that is modifying 'variable'
	return an empty set if 'variable' is not found
*/
unordered_set<int> PKB::getStmModifying(string variable)
{
	return mStore.getStmModifying(variable);
}

/*
	return the list of procedures that is modifying 'variable'
	return an empty set if 'variable' is not found
*/
unordered_set<string> PKB::getProcModifying(string variable)
{
	return mStore.getProcModifying(variable);
}

// returns a list of all Modifies pairs for statements
unordered_set<pair<int, string>, intStringhash> PKB::getStmVarModifyPairs()
{
	return mStore.getStmVarPairs();
}

// returns a list of all Modifies pairs for procedures
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
