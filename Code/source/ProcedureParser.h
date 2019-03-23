#pragma once
#include<string>
#include<vector>

#include "Statement.h"

class ProcedureParser {
public:
	static std::string parseProcName(const std::string toParse);
	static bool verifyProcName(const std::string toParse);
	void parseStmtLst(vector<Statement> stmtLst, std::string procedure);
};
