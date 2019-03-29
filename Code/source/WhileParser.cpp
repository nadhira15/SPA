#include "WhileParser.h"

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
  output: std::vector<std::string> of variables/constants
*/

WhileParser::WhileParser(int stmtNo, std::string stmt, std::vector<Statement> stmtlst, std::string procedure) {
	int i = stmt.find('(');
	int j = stmt.rfind(')');
	std::string cond_expr = stmt.substr(i + 1, j - i - 1);
	bool valid1 = !stmtlst.empty();
	//validate conditional expression and return all uses variable
	bool valid2 = ConditionalExp::verifyConditionalExp(cond_expr);
	if (valid1 && valid2) {
		var = ConditionalExp::getVariables();
		c = ConditionalExp::getConstants();
		this->procedure = procedure;
		stmtLst = stmtlst;
		stmtNum = stmtNo;
	} else {
		throw "While statement is not in the right form!\n";
	}
}

std::vector<std::string> WhileParser::getVariables() {
	return var;
}

std::vector<std::string> WhileParser::getConstants() {
	return c;
}
 
void WhileParser::parseStmtLst() {
	Parser().parse(stmtLst, stmtNum, procedure);
}
