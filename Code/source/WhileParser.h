#pragma once

#include <vector>
#include <stdio.h>
#include <string>
#include "Statement.h"
#include "PKB.h"
#include "ConditionalExp.h"
#include "Parser.h"

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
  output: std::vector<string> of variables/constants
*/

class WhileParser {
private:
	std::vector<std::string> var;
	std::vector<std::string> c;
	int stmtNum;
	std::string procedure;
	std::vector<Statement> stmtLst;
public:
	WhileParser(int stmtNo, std::string stmt, std::vector<Statement> stmtlst, std::string procedure);
	std::vector<std::string> getVariables();
	std::vector<std::string> getConstants();
	void parseStmtLst();
};

