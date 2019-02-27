#pragma once
#pragma once
#include<string>
#include<vector>

class PrintParser {
public:
	static std::string parsePrintStmt(const std::string toParse);
	static bool verifyPrintStmt(const std::string toParse);
};
