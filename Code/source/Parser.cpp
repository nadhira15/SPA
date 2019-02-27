#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "Parser.h"
#include "PKB.h"
#include "Statement.h"
#include "AssignParser.h"
#include "ReadParser.h"
#include "PrintParser.h"
#include "WhileParser.h"
#include "IfParser.h"
#include "ElseParser.h"

Parser::Parser()
{
}

int Parser::parse(vector<Statement> stmtLst, int parent, PKB pkb) {
	int prevStmtLine = 0;

	for (size_t i = 0; i < stmtLst.size(); i++) {
		Statement stmt = stmtLst.at(i);
		
		//Add Statement Type into PKB.
		populateStmtList(stmt, pkb);

		int currStmtLine = stmt.getStmtNum();

		//Add Follow relation if is not the very first line.
		if (prevStmtLine != 0) {
			pkb.addFollow(prevStmtLine, currStmtLine);
		}

		//Add Parent relation if parent is not 0.
		if (parent != 0) {
			pkb.addParent(parent ,currStmtLine);
		}

		populateDesignEntities(stmt, pkb);
	}
	
	return 0;
}

void Parser::populateDesignEntities(Statement stmt, PKB pkb) {
	string stmtString = stmt.getString();
	int stmtLine = stmt.getStmtNum();
	int stmtType = stmt.getType();

	// 1 "ASSIGN", 2 "CALL", 3 "READ", 4 "PRINT", 5 "WHILE", 6 "IF", 7 "ELSE", 8 "PROCEDURE"
	switch (stmtType) {
	case 1: {
		extractAssignEntity(stmtString, pkb, stmtLine);
		break;
	}
	case 2:
		//CallStatement
		//TODO: Iteration 2
		break;
	case 3:
		//ReadStatement
		extractReadEntity(stmtString, pkb, stmtLine);
		break;
	case 4:
		//PrintStatement
		extractPrintEntity(stmtString, pkb, stmtLine);
		break;
	case 5:
		//WhileStatement
		break;
	case 6:
		//IfStatemnt
		break;
	case 7:
		//ElseStmt
		break;
	case 8:
		//Procedure
		break;
	}
}

void Parser::extractAssignEntity(std::string &stmtString, PKB &pkb, int stmtLine) {
	AssignParser ap;
	string modified = ap.getLeft(stmtString);
	vector<string> usedVariables = ap.getRightVariable(stmtString);
	vector<string> usedConstants = ap.getRightConstant(stmtString);
	string prefixExpression = ap.getPrefixExpression(stmtString);

	//Add Variable and Modify
	pkb.addVariable(modified);
	pkb.addModifies(stmtLine, modified);

	//Add Constants
	for (string constant : usedConstants) {
		//TODO: Add constants.
	}

	//Add Variable and Uses.
	for (string variable : usedVariables) {
		pkb.addVariable(variable);
		pkb.addUses(stmtLine, variable);
	}

	pkb.addAssign(stmtLine, modified, prefixExpression);
}

void extractReadEntity(std::string &stmtString, PKB &pkb, int stmtLine) {
	ReadParser rp;
	string modified = rp.parseReadStmt(stmtString);

	pkb.addModifies(stmtLine, modified);
	pkb.addVariable(modified);
}

void extractPrintEntity(std::string &stmtString, PKB &pkb, int stmtLine) {
	PrintParser pp;
	string used = pp.parsePrintStmt(stmtString);

	pkb.addUses(stmtLine, used);
	pkb.addVariable(used);
}


void Parser::populateStmtList(Statement stmt, PKB pkb) {
	int stmtLine = stmt.getStmtNum();
	int stmtType = stmt.getType();

	// 1 "ASSIGN", 2 "CALL", 3 "READ", 4 "PRINT", 5 "WHILE", 6 "IF", 7 "ELSE", 8 "PROCEDURE"
	switch (stmtType) {
	case 1:
		//AssignStatement
		pkb.addStatement(stmtLine, stmType::assign);
		break;
	case 2:
		//CallStatement
		//TODO: Iteration 2
	case 3:
		//ReadStatement
		pkb.addStatement(stmtLine, stmType::read);
		break;
	case 4:
		//PrintStatement
		pkb.addStatement(stmtLine, stmType::print);
		break;
	case 5:
		//WhileStatement
		pkb.addStatement(stmtLine, stmType::whileStm);
		break;
	case 6:
		//IfStatemnt
		pkb.addStatement(stmtLine, stmType::ifStm);
		break;
	}
}