#include<stdio.h>
#include<string>
#include<vector>
#include<string>
#include<sstream>
#include<regex>
#include"PrintParser.h"

using namespace std;

//take in full statement string
std::string PrintParser::parsePrintStmt(const std::string toParse) {
	std::smatch matches;
	std::regex reg("\\s*print\\s+([a-zA-Z]+[a-zA-Z]*\\d*)\\s*");
	if (std::regex_match(toParse, matches, reg)) {
		std::ssub_match varName = matches[1];
		std::string name = varName.str();
		return name;
	}
	else {
		return "failed";
	}
}

bool PrintParser::verifyPrintStmt(const std::string toParse) {
	std::regex reg("\\s*print\\s+([a-zA-Z]+[a-zA-Z]*\\d*)\\s*");
	if (std::regex_match(toParse, reg)) {
		return true;
	}
	else {
		return false;
	}
}
