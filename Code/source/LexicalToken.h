#pragma once
#include <string>

using namespace std;

namespace LexicalToken {
	static bool verifyName(std::string s);
	static bool verifyInteger(std::string n);
}