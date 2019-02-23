#pragma once
#include<string>
#include<vector>

class ProcedureParser {
public:
	static std::string parseProcName(const std::string toParse);
	static bool varifyProcName(const std::string toParse);
};
