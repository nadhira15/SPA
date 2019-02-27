#pragma once

#include <vector>
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Statement.h"
#include "PKB.h"
#include "ConditionalExp.h"

using namespace std;

/* Usage guide for if, else and while parsers:

  If/while parser will contain:
  (1) constructor: to validate and store object variables
  (1) the parseStmtLst method: call parser to parse the stmtlst inside
  (2) getter methods for the Use variables and constants
  Else parser will only contain the parse method which calls parser to parse stmtlst inside

  parse method: ifParse, whileParse, elseParse
  input: stmtNo, stmt, stmtlst and pkb
  output: null

  getter methods: getVariable, getConstants
  input: null
  output: vector<string> of variables/constants
*/

class WhileParser {
private:
	vector<string> var;
	vector<string> c;
	int stmtNum;
	PKB pkb;
	vector<Statement> stmtLst;
public:
	WhileParser(int stmtNo, std::string stmt, vector<Statement> stmtlst, PKB pkb);
	vector<string> getVariables();
	vector<string> getConstants();
	void parseStmtLst();
};

