#include "CallParser.h"

std::regex validCallStatement("\\s*call\\s+(\\w+)\\s*");

std::string CallParser::parseCallStmt(std::string toParse) {
	if (verifyCallStmt(toParse) == false) {
		throw "Invalid Call Statement : " + toParse;
	}

	std::smatch matches;

	if (std::regex_match(toParse, matches, validCallStatement)) {
		std::ssub_match varName = matches[1];
		std::string name = varName.str();

		if (LexicalToken::verifyName(name)) {
			return name;
		}
		else {
			throw "Invalid Procedure Name : " + name;
		}
	}
	else {
		throw "Invalid Call Statement : " + toParse;
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
