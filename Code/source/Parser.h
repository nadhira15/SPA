#pragma once

#include<stdio.h>
#include<vector>

using namespace std;

#include "PKB.h"
#include "Statement.h"

class Parser
{

public:

	Parser();

	int parse(vector<Statement> stmtLst, int parent, PKB pkb);

	void populateDesignEntities(Statement stmt, PKB pkb);

	void extractAssignEntity(std::string &stmtString, PKB &pkb, int stmtLine);

	void populateStmtList(Statement stmt, PKB pkb);

};