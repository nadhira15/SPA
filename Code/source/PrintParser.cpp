#include<stdio.h>
#include<string>
#include<vector>
#include<string>
#include<sstream>
#include<regex>

using namespace std;
using namespace LexicalToken;

#include"PrintParser.h"
#include "LexicalToken.h"

//take in full statement string
std::string PrintParser::parsePrintStmt(const std::string toParse) {
	if (verifyPrintStmt(toParse)) {
		throw "Invalid Print Statement!";
	}
	std::smatch matches;
	std::regex reg("\\s*print\\s+(\\w+)\\s*");
	if (std::regex_match(toParse, matches, reg)) {
		std::ssub_match varName = matches[0];
		std::string name = varName.str();

		if (verifyName(name)) {
			return name;
		}
		else {
			throw "Invalid Variable Name"
		}
	}
	else {
		return "Invalid Print Statement!";
	}
}

bool PrintParser::verifyPrintStmt(const std::string toParse) {
	std::regex reg("\\s*print\\s+(\\w+)\\s*");
	if (std::regex_match(toParse, reg)) {
		return true;
	}
	else {
		return false;
	}
}
