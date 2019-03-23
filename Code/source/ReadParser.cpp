#include"ReadParser.h"

//take in full statement string
std::string ReadParser::parseReadStmt(const std::string toParse) {
	if (verifyReadStmt(toParse) == false) {
		throw "Invalid Read Statement";
	}
	std::smatch matches;
	std::regex reg("\\s*read\\s+(\\w+)\\s*");
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
		throw "Invalid Read Statement";
	}
}

bool ReadParser::verifyReadStmt(const std::string toParse) {
	std::regex reg("\\s*read\\s+(\\w+)\\s*");
	if (std::regex_match(toParse,reg)) {
		return true;
	}
	else {
		return false;
	}
}
