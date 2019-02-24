#pragma once

#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

#include "FollowStorage.h"

enum stmType { read, print, assign, whileStm, ifStm };

class PKB {
public:
	PKB();

	//general adder methods
	void addProc(string procName);
	void addStatement(int stmNo, stmType type);
	void addVariable(string name);
	void addConstant(int value);

	//adding follows relationships
	bool addFollow(int stm1, int stm2);
	bool addFollow_S(int stm1, int stm2);

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

	//For follow relations
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
};