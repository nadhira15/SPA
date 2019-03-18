#include "CallParser.h"

std::regex validCallStatement("\\s*call\\s+(\\w+)\\s*");

//take in full statement string
std::string CallParser::parseCallStmt(std::string toParse) {
	if (verifyCallStmt(toParse) == false) {
		throw "Invalid Read Statement";
	}
	std::smatch matches;

	if (std::regex_match(toParse, matches, validCallStatement)) {
		std::ssub_match varName = matches[1];
		std::string name = varName.str();

		if (LexicalToken::verifyName(name)) {
			return name;
		}
		else {
			throw "Invalid Procedure Name";
		}
	}
	else {
		throw "Invalid Read Statement";
	}
}

bool CallParser::verifyCallStmt(std::string toParse) {
	if (std::regex_match(toParse, validCallStatement)) {
		return true;
	}
	else {
		return false;
	}
}
