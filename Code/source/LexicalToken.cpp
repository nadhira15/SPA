#include <string>
#include "LexicalToken.h"

using namespace std;
using namespace LexicalToken;

/*Check if string is a NAME Lexical. A Name Lexical starts with a alphabet and is followed by a string of alphanumerical
 *Characters. Returns true if input is suitable as a procedure name or variable name. Returns false otherwise.
 */
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

/*Checks if string is an integer by trying checking if every character in the string
 *is a digit. Returns true if every character is a digit and returns false if 1 of them
 *is not a digit
 */
bool LexicalToken::verifyInteger(std::string number){
	for (size_t i = 0; i < number.length(); i++) {
		if (!isdigit(number[i])) {
			return false;
		}
	}
	return true;
}