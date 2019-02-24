#pragma once

#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

#include "FollowStorage.h"
#include "ParentStorage.h"
#include "UseStorage.h"

enum stmType { read, print, assign, whileStm, ifStm };
enum argType { stm, proc };

class PKB {
public:
	PKB();

	//general adder methods
	void addProc(string procName);
	void addStatement(int stmNo, stmType type);
	void addVariable(string name);
	void addConstant(int value);

	//adding Follows relationships
	bool addFollow(int stm1, int stm2);
	bool addFollow_S(int stm1, int stm2);
	bool setAllFollowing(int stm, unordered_set<int> stmList);
	bool setAllFollowBy(int stm, unordered_set<int> stmList);

	//adding Parent relationships
	bool addParent(int stm1, int stm2);
	bool addParent_S(int stm1, int stm2);
	bool setAllAncestors(int stm, unordered_set<int> stmList);
	bool setAllDescendants(int stm, unordered_set<int> stmList);

	//adding Use relationships
	bool addUses(int stm, string variable);
	bool addUses(string procedure, string variable);

	//general getter methods
	string getProcName();
	int getTotalStmNo();
	stmType getStmType(int stm);
	unordered_set<int> getReadStms();
	unordered_set<int> getPrintStms();
	unordered_set<int> getAssignStms();
	unordered_set<int> getIfStms();
	unordered_set<int> getWhileStms();
	unordered_set<string> getVariables();
	unordered_set<int> getConstants();

	//For Follows/Follows* relations
	bool hasFollowRelation();
	bool hasFollow_S_Pair(int stm1, int stm2);
	int getPrvStm(int stm);
	int getNxtStm(int stm);
	unordered_set<int> getAllFollowing(int stm);
	unordered_set<int> getAllFollowedBy(int stm);
	unordered_set<int> getAllFollowers();
	unordered_set<int> getAllFollowed();
	unordered_set< pair<int, int> > getFollowPairs();
	unordered_set< pair<int, int> > getFollow_S_Pairs();

	//For Parent/Parent* relations
	bool hasParentRelation();
	bool isParent(int stm);
	bool isChild(int stm);
	bool hasAncDescPair(int stm1, int stm2);
	int getParent(int stm);
	unordered_set<int> getChildren(int stm);
	unordered_set<int> getAllAncestors(int stm);
	unordered_set<int> getAllDescendants(int stm);
	unordered_set<int> getAllParents();
	unordered_set<int> getAllChildren();
	unordered_set< pair<int, int> > getParentChildPairs();
	unordered_set< pair<int, int> > getAncDescPairs();

	//For Uses relations
	bool isUsing(int stm, string variable);
	bool isUsing(string procedure, string variable);
	unordered_set<string> getUsedVar(int stm);
	unordered_set<string> getUsedVar(string procedure);
	unordered_set<int> getStmUsing(string variable);
	unordered_set<string> getProcUsing(string variable);
	unordered_set< pair<int, string> > getStmVarUsePairs();
	unordered_set< pair<string, string> > getProcVarUsePairs();

private:
	static string procName;
	static vector<stmType> stmTypeList;
	static unordered_set<string> varList;
	static unordered_set<int> constList;
	static unordered_set<int> readStmList;
	static unordered_set<int> printStmList;
	static unordered_set<int> assignStmList;
	static unordered_set<int> ifStmList;
	static unordered_set<int> whileStmList;

	static FollowStorage fStore;
	static ParentStorage pStore;
	static UseStorage uStore;
};