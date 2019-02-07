#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class PKB {
public:
	PKB();

	void addStatement(NodeType type, string variable);

	short PrvLn(short stm);
	short NxtLn(short stm);
	vector<short> getAllFollowing(short stm);
	vector<short> getAllFollowedBy(short stm);
	vector<short> getAllFollowers();
	vector<short> getAllFollowed();

	vector<string> getAllVariable();

private:
	static vector<TNode*> stmTable;
	static vector<string> varTable;

	void addVariable(string name);
};