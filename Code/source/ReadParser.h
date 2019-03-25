#pragma once
#include <regex>
#include <string>
#include <vector>

#include "LexicalToken.h"

class ReadParser {
public:
	static std::string parseReadStmt(const std::string toParse);
	static bool verifyReadStmt(const std::string toParse);
};