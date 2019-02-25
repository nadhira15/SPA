#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "WhileParser.h"
#include "Statement.h"

using namespace std;

string WhileParser::parse(short stmtNo, std::string stmt, vector<Statement> stmtlst) {
	int i = stmt.find('(');
	int j = stmt.rfind(')');
	string cond_expr = stmt.substr(i + 1, j - i - 1);
	//validate conditional expression and return all uses variable

	//call parser for while

}
