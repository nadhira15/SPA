#pragma once
#include "Parser.h"

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

		//Add Follow relation if is not the very first line or if not an else statement
		if (prevStmtLine != 0 && stmt.getType() != 7) {
			pkb.addFollow(prevStmtLine, currStmtLine);
		}

		//Add Parent relation if parent is not 0.
		if (parent != 0) {
			pkb.addParent(parent ,currStmtLine);
		}

		//Add VariableName, Constants, and Procedure name into PKB.
		populateDesignEntities(stmt, pkb);
		
		//Update previous statement line.
		prevStmtLine = currStmtLine;

		}
	
	return 0;
}

void Parser::populateDesignEntities(Statement stmt, PKB pkb) {
	std::string stmtString = stmt.getString();
	int stmtLine = stmt.getStmtNum();
	int stmtType = stmt.getType();

	// 1 "ASSIGN", 2 "CALL", 3 "READ", 4 "PRINT", 5 "WHILE", 6 "IF", 7 "ELSE", 8 "PROCEDURE"
	switch (stmtType) {
	case 1: {
		//Assign Statement
		extractAssignEntity(stmtString, pkb, stmtLine);
		break;
	}
	case 2:
		//Call Statement
		extractCallEntity(stmtString, pkb, stmtLine);
		break;
	case 3: {
		//ReadStatement
		extractReadEntity(stmtString, pkb, stmtLine);
		break;
	}
	case 4: {
		//PrintStatement
		extractPrintEntity(stmtString, pkb, stmtLine);
		break;
	}
	case 5: {
		//WhileStatement
		vector<Statement> whileStmtLst = stmt.getStmtLst();
		extractWhileEntity(stmtString, pkb, stmtLine, whileStmtLst);
		break;
	}
	case 6: {
		//IfStatement
		vector<Statement> ifStmtLst = stmt.getStmtLst();
		extractIfEntity(stmtString, pkb, stmtLine, ifStmtLst);
		break;
	}
	case 7: {
		//ElseStmt 
		vector<Statement> elseStmtLst = stmt.getStmtLst();
		extractElseEntity(stmtString, pkb, stmtLine, elseStmtLst);
		break;
	}
	case 8: {		//Procedure
		vector<Statement> procStmtLst = stmt.getStmtLst();
		extractProcedureEntity(stmtString, pkb, procStmtLst);
	}
	}
}

void Parser::extractAssignEntity(std::string &stmtString, PKB &pkb, int stmtLine) {
	AssignParser ap;
	std::string modified = ap.getLeft(stmtString);
	std::vector<std::string> usedVariables = ap.getRightVariable(stmtString);
	std::vector<std::string> usedConstants = ap.getRightConstant(stmtString);
	std::string prefixExpression = ap.getPrefixExpression(stmtString);

	//Add Variable and Modify
	pkb.addVariable(modified);
	pkb.addModifies(stmtLine, modified);

	//Add Constants
	for (string constant : usedConstants) {
		pkb.addConstant(constant);
	}

	//Add Variable and Uses.
	for (string variable : usedVariables) {
		pkb.addVariable(variable);
		pkb.addUses(stmtLine, variable);
	}

	pkb.addAssign(stmtLine, modified, prefixExpression);
}

void Parser::extractCallEntity(std::string &stmtString, PKB &pkb, int stmtLine) {
	CallParser cp;
	string procedureName = cp.parseCallStmt(stmtString);
	
	pkb.addProc(procedureName);
}

void Parser::extractReadEntity(std::string &stmtString, PKB &pkb, int stmtLine) {
	ReadParser rp;
	string modified = rp.parseReadStmt(stmtString);

	pkb.addModifies(stmtLine, modified);
	pkb.addVariable(modified);
}

void Parser::extractPrintEntity(std::string &stmtString, PKB &pkb, int stmtLine) {
	PrintParser pp;
	string used = pp.parsePrintStmt(stmtString);

	pkb.addUses(stmtLine, used);
	pkb.addVariable(used);
}

void Parser::extractWhileEntity(std::string &stmtString, PKB &pkb, int stmtLine, vector<Statement> stmtLst) {
	WhileParser wp = WhileParser(stmtLine, stmtString, stmtLst, pkb);
	vector<string> constants = wp.getConstants();
	vector<string> variables = wp.getVariables();

	for (string variable : variables) {
		pkb.addVariable(variable);
		pkb.addUses(stmtLine, variable);
	}

	for (string constant : constants) {
		pkb.addConstant(constant);
	}
	
	wp.parseStmtLst();
}

void Parser::extractIfEntity(std::string &stmtString, PKB &pkb, int stmtLine, vector<Statement> stmtLst) {
	IfParser ip = IfParser(stmtLine, stmtString, stmtLst, pkb);
	vector<string> constants = ip.getConstants();
	vector<string> variables = ip.getVariables();

	for (string variable : variables) {
		pkb.addVariable(variable);
		pkb.addUses(stmtLine, variable);
	}

	for (string constant : constants) {
		pkb.addConstant(constant);
	}


	ip.parseStmtLst();
}

void Parser::extractElseEntity(std::string &stmtString, PKB &pkb, int stmtLine, vector<Statement> stmtLst) {
	ElseParser ep = ElseParser(stmtLine, stmtString, stmtLst, pkb);
	ep.parseStmtLst();
}

void Parser::extractProcedureEntity(std::string &stmtString, PKB &pkb, vector<Statement> stmtLst) {
	ProcedureParser pp;
	string procName = pp.parseProcName(stmtString);
	pkb.addProc(procName);

	pp.parseStmtLst(stmtLst, pkb);
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
		pkb.addStatement(stmtLine, stmType::call);
		break;
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