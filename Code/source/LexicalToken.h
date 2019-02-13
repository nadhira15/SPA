#pragma once
#include <string>

class LexicalToken {
public:
	static bool verifyName(std::string s);
	static bool verifyInteger(std::string n);
};