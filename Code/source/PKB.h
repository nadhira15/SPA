#pragma once

#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

#include "FollowStorage.h"
#include "ParentStorage.h"
#include "UseStorage.h"
#include "ModifyStorage.h"
#include "Hasher.h"

enum stmType { read, print, assign, whileStm, ifStm };

/*
	Accepts relationship, pattern and other general data from Parser and DesignExtractor and
	stores them here or into their respective Storage classes.
	Reply to queries made by the QueryEvaluator.
*/
class PKB {
public:
	PKB();

	//general adder methods
	void addProc(string procName);
	void addStatement(int stmNo, stmType type);
	void addVariable(string name);
	void addConstant(string value);

	//adding Follows relationships
	bool addFollow(int stm1, int stm2);
	bool setFollowers(int stm, unordered_set<int> stmList);
	bool setStmFollowedBy(int stm, unordered_set<int> stmList);

	//adding Parent relationships
	bool addParent(int stm1, int stm2);
	bool setAncestors(int stm, unordered_set<int> stmList);
	bool setDescendants(int stm, unordered_set<int> stmList);

	//adding Uses relationships
	bool addUsesStm(int stm, string variable);
	bool addUsesProc(string procedure, string variable);

	//adding Modifies relationships
	bool addModifiesStm(int stm, string variable);
	bool addModifiesProc(string procedure, string variable);

	//adding patterns
	bool addAssign(int stm, string variable, string expr);

	//general getter methods
	unordered_set<string> getProcList();
	int getTotalStmNo();
	stmType getStmType(int stm);
	unordered_set<int> getReadStms();
	unordered_set<int> getPrintStms();
	unordered_set<int> getAssignStms();
	unordered_set<int> getIfStms();
	unordered_set<int> getWhileStms();
	unordered_set<string> getVariables();
	unordered_set<string> getConstants();

	//For Follows/Follows* relations
	bool hasFollowRelation();
	bool hasFollowStarPair(int stm1, int stm2);
	int getStmFollowedBy(int stm);
	int getFollower(int stm);
	unordered_set<int> getAllFollowing(int stm);
	unordered_set<int> getAllFollowedBy(int stm);
	unordered_set<int> getAllFollowers();
	unordered_set<int> getAllFollowed();
	unordered_set< pair<int, int>, intPairhash> getFollowPairs();
	unordered_set< pair<int, int>, intPairhash> getFollowStarPairs();

	//For Parent/Parent* relations
	bool hasParentRelation();
	bool isParent(int stm);
	bool isChild(int stm);
	bool hasAncDescPair(int stm1, int stm2);
	int getParent(int stm);
	unordered_set<int> getChildren(int stm);
	unordered_set<int> getAncestors(int stm);
	unordered_set<int> getDescendants(int stm);
	unordered_set<int> getAllParents();
	unordered_set<int> getAllChildren();
	unordered_set< pair<int, int>, intPairhash> getParentChildPairs();
	unordered_set< pair<int, int>, intPairhash> getAncDescPairs();

	//For Uses relations
	bool isUsing(int stm, string variable);
	bool isUsing(string procedure, string variable);
	unordered_set<string> getVarUsedByStm(int stm);
	unordered_set<string> getVarUsedByProc(string procedure);
	unordered_set<int> getStmUsing(string variable);
	unordered_set<string> getProcUsing(string variable);
	unordered_set< pair<int, string>, intStringhash> getStmVarUsePairs();
	unordered_set< pair<string, string>, strPairhash> getProcVarUsePairs();

	//For Modifies relations
	bool isModifying(int stm, string variable);
	bool isModifying(string procedure, string variable);
	unordered_set<string> getVarModifiedByStm(int stm);
	unordered_set<string> getVarModifiedByProc(string procedure);
	unordered_set<int> getStmModifying(string variable);
	unordered_set<string> getProcModifying(string variable);
	unordered_set< pair<int, string>, intStringhash> getStmVarModifyPairs();
	unordered_set< pair<string, string>, strPairhash> getProcVarModifyPairs();

	//For Pattern clauses
	unordered_set<int> findPattern(string variable, string expr, bool isExclusive);
	unordered_set<int> findPattern(string expr, bool isExclusive);
	unordered_set<pair<int, string>, intStringhash> findPatternPairs(string expr, bool isExclusive);

private:
	static unordered_set<string> procList;
	static vector<stmType> stmTypeList;
	static unordered_set<string> varList;
	static unordered_set<string> constList;
	static unordered_set<int> readStmList;
	static unordered_set<int> printStmList;
	static unordered_set<int> assignStmList;
	static unordered_set<int> ifStmList;
	static unordered_set<int> whileStmList;

	static FollowStorage fStore;
	static ParentStorage pStore;
	static UseStorage uStore;
	static ModifyStorage mStore;
	static unordered_map<int, pair<string, string> > patternList;
};