#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "TNode.h"
#include "Statement.h"

int parse(vector<Statement> stmtLst, int parent) {
	int prevStmtLine = 0;

	for (size_t i = 0; i < stmtLst.size(); i++) {
		Statement stmt = stmtLst.at(i);

		int currStmtLine = stmt.getStmtNum();

		if (prevStmtLine != 0) {
			//Add to PKB
		}
	}
	
	return 0;
}