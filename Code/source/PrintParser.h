#pragma once
#include <regex>
#include <string>
#include <vector>

#include "LexicalToken.h"

class PrintParser {
public:
	static std::string parsePrintStmt(const std::string toParse);
	static bool verifyPrintStmt(const std::string toParse);
};
