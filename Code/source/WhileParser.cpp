#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "WhileParser.h"
#include "Statement.h"
#include "PKB.h"

using namespace std;

vector<string> WhileParser::whileParse(short stmtNo, std::string stmt, vector<Statement> stmtlst) {
	int i = stmt.find('(');
	int j = stmt.rfind(')');
	string cond_expr = stmt.substr(i + 1, j - i - 1);
	//validate conditional expression and return all uses variable
	vector<string> variables; //= validateCondExpr(cond_expr);

	//call parser for while
	//Parser(stmtlst);
	
	return variables;
}
