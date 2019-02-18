#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

#include "Statement.h"
#include "Preprocesser.h"

string chunk;
int count1 = 0;
int count2 = 0;
int stopper = 0;
vector<Statement> procLst;

/* returns procedure vector */
vector<Statement> getProcLst() {
	return procLst;
}

/*
input: nil
output: vector<Statement> that contains procedure statements
*/
void Processer(string chunk1)
{
	chunk = chunk1;
	size_t prev = chunk.find("procedure "), next;
	while (prev != string::npos) {
		count1 = 0;
		count2 = 0;
		next = chunk.find("procedure ", prev + 10);
		if (next == string::npos) {
			procLst.push_back(processProc(prev, chunk.size()));
		}
		else {
			procLst.push_back(processProc(prev, next));
		}
		prev = next;
	}
}

/*
input: start, end (not inclusive)
output: Statement
NOTE procedure can only occur once
*/
Statement processProc(int bookmark, int last)
{
	vector<Statement> stmtLst;
	int valid = -1;
	string tmp;
	size_t tmpn, pos;
	//check for last '}'
	pos = chunk.find_last_not_of(" \t\f\v\n\r", last - 1);
	if (chunk[last] != '}') {
		valid = 0;
	}
	if (chunk.find('{') != string::npos) {
		tmpn = chunk.find('{');
		tmp = trim(chunk.substr(bookmark, tmpn - bookmark));
		valid = validateProc(tmp);
	}
	if (valid == 0) {} //Halt the program and show error?
	Statement s = Statement(tmp, processLst(tmpn + 1, pos), 7);
	if (count1 != count2) {} //
	return s;
}

/*
input: start, end (not inclusive and estimate only)
output: Statement
*/
vector<Statement> processLst(int bookmark, int last) {
	vector<Statement> stmtlst;
	int valid, i;
	string tmp;
	string tmp2;
	for (i = bookmark; i < last; i++) {
		if (chunk[i] == '}') {
			count2++;
			stopper = i + 1;
			i = last;
		}
		else if (chunk[i] == '{') {
			count1++;
			tmp = trim(chunk.substr(bookmark, i - 1 - bookmark));
			valid = validateCurvedBrackets(tmp);
			if (valid == 0) break; //
			stmtlst.push_back(Statement(tmp, processLst(i + 1, last), valid));
			i = stopper;
			bookmark = stopper;
		}
		else if (chunk[i] == ';') {
			tmp = trim(chunk.substr(bookmark, i - bookmark));
			bookmark = i + 1;
			valid = validateSemicolon(tmp);
			if (valid == 0) break; //
			stmtlst.push_back(Statement(tmp, 0));
		}
	}
	return stmtlst;
}

/*
types: { "ASSIGN", "CALL", "READ", "PRINT" } as of iter #1
input: trimmed string
output: type[0-invalid, 1-read/print/call, 2-assign]
validation of syntax errors
errors covered: incorrect syntax
errors NOT covered: incorrect variables, expression (i.e. *+/-=)
*/
int validateSemicolon(string s)
{
	int result = 0;
	if (s.find("=") != string::npos) {
		//ASSIGN is passed as long as it contains an "="
		result = 1;
	}
	string firstWord = s.substr(0, s.find(" "));
	s = trim(s.substr(4, s.size() - 4));
	if (s.find(" ") == string::npos) {
		//READ, CALL, PRINT is passed as long as it contains 2 words
		if (firstWord.compare("call") == 0) result = 2;
		else if (firstWord.compare("read") == 0) result = 3;
		else if (firstWord.compare("print") == 0) result = 4;
	}
	return result;
}

/*
types: { "IF", "WHILE" } as of iter #1
validation of syntax errors
errors covered: incorrect syntax for if, while
errors NOT covered: incorrect variables, expression (i.e. *+/-=), non-alphanumeric characters
*/
int validateCurvedBrackets(string s)
{
	int result = 0;
	string firstWord = s.substr(0, s.find(" "));
	if (firstWord.compare("while") == 0) {
		//WHILE is passed as long as it is followed by '(' and ')'
		s = trim(s.substr(5, s.size() - 5));
		if (s[0] == '(' && s.find(')') != string::npos) result = 5;
	}
	else if (firstWord.compare("if") == 0) {
		//IF is passed as long as it is followed by '(' and ')' and last word is THEN
		s = trim(s.substr(2, s.size() - 2));
		string lastWord = s.substr(s.rfind(" ") + 1, s.size() - s.rfind(" "));
		if (s[0] == '(' && s.find(')') != string::npos && lastWord.compare("then") == 0) result = 6;
	}
	else if (firstWord.compare("else") == 0) {
		if (trim(s).size() == 4) result = 6;
	}
	return result;
}

/*
types: { "PROCEDURE" } as of iter #1
validation of syntax errors
errors covered: incorrect syntax for procedure
errors NOT covered: incorrect variables, expression (i.e. *+/-=), non-alphanumeric characters
*/
int validateProc(string s)
{
	int result = 0;
	string firstWord = s.substr(0, s.find(" "));
	if (firstWord.compare("procedure") == 0) {
		//PROCEDURE is passed as long as it contains 2 words
		s = trim(s.substr(9, s.size() - 9));
		if (s.find(" ") == string::npos) result = 7;
	}
	return result;
}

/*
trim whitespace from front and back of string
adapted from an answer in StackOverflow
https://stackoverflow.com/questions/44973435/stdptr-fun-replacement-for-c17/44973498#44973498
*/
string trim(string s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
		return !isspace(ch);
	}));
	s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
		return !isspace(ch);
	}).base(), s.end());
	return s;
}
;