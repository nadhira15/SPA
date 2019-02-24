#pragma once

#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

enum stmType { read, print, assign, whileStm, ifStm };

class PKB {
public:
	PKB();

	void addProc(string procName);
	void addStatement(int stmNo, stmType type);
	void addVariable(string name);
	void addConstant(int value);

	string getProcName();
	int getTotalStmNo();
	unordered_set<int> getReadStms();
	unordered_set<int> getPrintStms();
	unordered_set<int> getAssignStms();
	unordered_set<int> getIfStms();
	unordered_set<int> getWhileStms();
	vector<string> getVariables();
	vector<int> getConstants();

private:
	static string procName;
	static vector<stmType> stmTypeList;
	static vector<string> varList;
	static vector<int> constList;
	static unordered_set<int> readStmList;
	static unordered_set<int> printStmList;
	static unordered_set<int> assignStmList;
	static unordered_set<int> ifStmList;
	static unordered_set<int> whileStmList;
};