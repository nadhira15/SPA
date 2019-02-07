#include <string>

using namespace std;

#include "LexicalToken.h"

//Check if string is a NAME Lexical. A Name Lexical starts with a alphabet and is followed by a string of alphanumerical
//Characters. Returns true if input is suitable as a procedure name or variable name. Returns false otherwise.
bool LexicalToken::verifyName(std::string name) {

	//Check if first character is an alphabet
	if (!isalpha(name[0])) {
		return false;
	}

	//Check if subsequent characters are alphanumericals.
	//TODO: Check for last character.
	for (size_t i = 1; i < name.length(); i++) {
		if (!isalnum(name[i])) {
			return false;
		}
	}

	return true;
}

//Checks if string is an integer by trying to convert it into an long variable. If conversion succeeds,
//then it is a decimal integer and returns true 
bool LexicalToken::verifyInteger(std::string number){
	char* p;

	long checkInteger = strtol(number.c_str(), &p, 10);

	if (*p) {
		return false;
	}
	else {
		return true;
	}
}