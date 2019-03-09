#pragma once
#include <string>

using namespace std;

/* Contains a suite of verifiers to check simple Lexical Tokens in accordance
 * to the SPA program. These includes the check on
 * 
 * (1) Verification that a name for variable or procedure starts with an alphabet
 *     and that the remaining are either alphabets or numbers.
 * (2) Verification that a string contains a valid Integer so that it can be considered
 *     a constant.
 */
namespace LexicalToken {
	/* verifyName checks that a given string contains a valid Name
	 * A valid Name is one that is of the form LETTER(LETTER|DIGIT)*
	 */
	bool verifyName(std::string s);

	/* verifyName checks that a given string contains a valid INTEGER
	 * A valid Integer is one that is of the form DIGIT+
	 */
	bool verifyInteger(std::string n);
}