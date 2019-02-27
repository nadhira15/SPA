#pragma once
#pragma once
#include<string>
#include<vector>

class ReadParser {
public:
	static std::string parseReadStmt(const std::string toParse);
	static bool verifyReadStmt(const std::string toParse);
};