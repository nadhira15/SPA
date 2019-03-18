#pragma once

#include<stdio.h>
#include<vector>

#include "PKB.h"
#include "Statement.h"
#include "AssignParser.h"
#include "CallParser.h"
#include "ReadParser.h"
#include "PrintParser.h"
#include "WhileParser.h"
#include "IfParser.h"
#include "ElseParser.h"
#include "ProcedureParser.h"

class Parser
{

public:

	Parser();

	int parse(vector<Statement> stmtLst, int parent, PKB pkb);

private:

	void populateDesignEntities(Statement stmt, PKB pkb);

	void extractAssignEntity(std::string &stmtString, PKB &pkb, int stmtLine);

	void extractCallEntity(std::string & stmtString, PKB & pkb, int stmtLine);

	void extractReadEntity(std::string & stmtString, PKB & pkb, int stmtLine);

	void extractPrintEntity(std::string & stmtString, PKB & pkb, int stmtLine);

	void extractWhileEntity(std::string & stmtString, PKB & pkb, int stmtLine, vector<Statement> stmtLst);

	void extractIfEntity(std::string & stmtString, PKB & pkb, int stmtLine, vector<Statement> stmtLst);

	void extractElseEntity(std::string & stmtString, PKB & pkb, int stmtLine, vector<Statement> stmtLst);

	void extractProcedureEntity(std::string & stmtString, PKB & pkb, vector<Statement> stmtLst);

	void populateStmtList(Statement stmt, PKB pkb);

};