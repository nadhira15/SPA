#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "IfParser.h"
#include "Statement.h"

using namespace std;

string IfParser::parse(short stmtNo, string stmt, vector<Statement> stmtlst) {
	int i = stmt.find('(');
	int j = stmt.rfind(')');
	string cond_expr = stmt.substr(i + 1, j - i - 1);
	//validate conditional expression and return all uses variable

	//call if parser

}
