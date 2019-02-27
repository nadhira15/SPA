#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Parser.h"
#include "ElseParser.h"
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

ElseParser::ElseParser(int stmtNo, string stmt, vector<Statement> stmtlst, PKB pkb1) {
	stmtNum = stmtNo;
	stmtLst = stmtlst;
	pkb = pkb1;
}

// else does not require secondary validation

void ElseParser::parseStmtLst() {
	Parser().parse(stmtLst, stmtNum, pkb);
}
