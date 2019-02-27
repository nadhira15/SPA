#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "IfParser.h"
#include "Statement.h"
#include "PKB.h"
#include "ConditionalExp.h"

using namespace std;

/* Usage guide for if, else and while parsers:

  If/while parser will contain:
  (1) the parse method: to validate and call parser to parse the stmtlst inside
  (2) getter methods for the Use variables and constants
  Else parser will only contain the parse method which calls parser to parse stmtlst inside

  parse method: ifParse, whileParse, elseParse
  input: stmtNo, stmt, stmtlst and pkb
  output: null

  getter methods: getVariable, getConstants
  input: null
  output: vector<string> of variables/constants
*/

IfParser::IfParser(short stmtNo, string stmt, vector<Statement> stmtlst, PKB pkb) {
	int i = stmt.find('(');
	int j = stmt.rfind(')');
	string cond_expr = stmt.substr(i + 1, j - i - 1);
	//validate conditional expression and return all uses variable
	bool valid = verifyConditionalExp(cond_expr);
	if (valid) {
		var = ConditionalExp::getVariables();
		c = ConditionalExp::getConstants();
		//call parser for if stmtlst
		//Parser(stmtlst, pkb);
	}
	else {
		throw "If statement is not in the right form!\n";
	}
}

vector<string> IfParser::getVariables() {
	return var;
}

vector<string> IfParser::getConstants() {
	return c;
}
