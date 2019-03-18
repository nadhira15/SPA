#include "PrintParser.h"

//take in full statement string
std::string PrintParser::parsePrintStmt(const std::string toParse) {
	if (verifyPrintStmt(toParse) == false) {
		throw "Invalid Print Statement!";
	}
	std::smatch matches;
	std::regex reg("\\s*print\\s+(\\w+)\\s*");
	if (std::regex_match(toParse, matches, reg)) {
		std::ssub_match varName = matches[1];
		std::string name = varName.str();

		if (LexicalToken::verifyName(name)) {
			return name;
		}
		else {
			throw "Invalid Variable Name";
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
