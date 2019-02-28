#include<stdio.h>
#include<string>
#include<vector>
#include<string>
#include<sstream>
#include<regex>

using namespace std;

#include "ProcedureParser.h"
#include "LexicalToken.h"
#include "Parser.h"

using namespace LexicalToken;

//take in full statement string
std::string ProcedureParser:: parseProcName(const std::string toParse){
	if (verifyProcName(toParse) == false) {
		throw "Invalid Procedure Declaration!";
	}

	std::smatch matches;
	std::regex reg("\\s*procedure\\s+(\\w+)\\s*");
	if (std::regex_match(toParse, matches, reg)) {
		std::ssub_match procedureName = matches[1];
		std::string name = procedureName.str();

		if (verifyName(name)) {
			return name;
		}
		else {
			throw "Invalid Variable Name!";
		}
	}
	else {
		throw "Invalid Procedure Declaration!";
	}
}

bool ProcedureParser::verifyProcName(const std::string toParse) {
	std::regex reg("\\s*procedure\\s+(\\w+)\\s*");
	if (std::regex_match(toParse, reg)) {
		return true;
	}
	else {
		return false;
	}
}

void ProcedureParser::parseStmtLst(vector<Statement> stmtLst, PKB pkb) {
	Parser().parse(stmtLst, 0, pkb);
}