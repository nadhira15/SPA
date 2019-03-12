#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

#include "Statement.h"
#include "Preprocessor.h"

/**** Usage guide
  Preprocessor will contain:
  (1) validate(...): the validator methods to validate statement types (used mainly for testing)
  (2) getProcLst: getter method for the procedure list
  (3) Preprocessor: constructor method to satrt the processing of source code

  Preprocessor:
  input: string representing source code
  output: the object

  getter methods: getProcLst
  input: null
  output: vector<Statement> representing the vector of procedures in the program

  to use:
  (1) create preprocesser and get procedure list: 
  Preprocessor p = Preprocessor(string chunk);
  p.process();
  vector<Statement> procLst = p.getProcLst();
*/

vector<Statement> Preprocessor::getProcLst() {
	return procLst;
}

/* constructor
input: nil
output: vector<Statement> that contains procedure statements
*/
Preprocessor::Preprocessor(string chunk1)
{
	//take file contents and save as global variable
	chunk1.erase(chunk1.find('\0'));
	chunk = trim(chunk1);
	size = chunk.size();
}

void Preprocessor::process() {
	int bookmark = 0;
	try {
		while (stopper < size - 1) { //chunk is trimmed so last item should be }
			procLst.push_back(processProc(bookmark));
			bookmark = stopper + 1;
		}
	} catch (string e) {
		string s1 = "Pre-processing error, incorrect syntax for ";
		cout << s1 << e << "! \n";
	}
}

/*
input: start, end (not inclusive)
output: Statement
NOTE 'procedure' can only occur once per procedure
*/
Statement Preprocessor::processProc(int bookmark)
{
	int valid;
	string tmp;
	size_t tmpn;
	int tmpn2;
	tmpn = chunk.find('{', bookmark);
	if (tmpn != string::npos) {
		tmp = trim(chunk.substr(bookmark, tmpn - bookmark));
		valid = validateProc(tmp); //validate procedure term
	} else {
		valid = 0; // unable to find '{'
	}
	if (valid == 0) {
		throw "procedure";
	}
	tmpn2 = stmtNum;
	vector<Statement> stmtlst = processLst(tmpn + 1);
	if (stmtlst.empty() || !ifStmt.empty()) {
		throw "procedure";
	}
	Statement s = Statement(tmp, stmtlst, valid, tmpn2); //will proceed even with errors
	return s;
}

/*
input: start, end (usually })
output: Statement
*/
vector<Statement> Preprocessor::processLst(int bookmark) {
	vector<Statement> stmtlst;
	int valid, i, tmpN;
	string tmp, tmp2;
	stopper = 0;
	for (i = bookmark; i < size; i++) {
		if (chunk[i] == '}') { //end of a {} part
			//reset stopper
			stopper = i;
			//end loop by breaking
			break;
		}
		else if (chunk[i] == '{') {
			//return trimmed string of statement
			tmp = trim(chunk.substr(bookmark, i - bookmark));
			//check validity
			valid = validateCurvedBrackets(tmp);
			//based on results of validation
			if (valid == 0) {
				throw "if/else, while";
			} else if (valid == 7) {
				// ELSE statement
				if (ifStmt.empty()) {
					throw "if/else";
				} else {
					tmpN = ifStmt.top();
					ifStmt.pop();
				}
			} else if (valid == 6) {
				// IF statement
				ifStmt.push(stmtNum);
			}
			if (valid == 5 || valid == 6) {
				// WHILE statement
				tmpN = stmtNum;
				stmtNum++;
			}
			stmtlst.push_back(Statement(tmp, processLst(i + 1), valid, tmpN));
			//after execution of previous process {}, return to previous bookmark and i-count
			i = stopper;
			bookmark = stopper + 1;
		}
		else if (chunk[i] == ';') {
			tmp = trim(chunk.substr(bookmark, i - bookmark));
			bookmark = i + 1;
			tmpN = stmtNum;
			valid = validateSemicolon(tmp);
			if (valid == 0) {
				throw "assign, call, print, read";
			} else {
				stmtlst.push_back(Statement(tmp, valid, tmpN));
				stmtNum++;
			}
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
int Preprocessor::validateSemicolon(string s)
{
	int result = 0;
	size_t end;
	if ((s.find("=") != string::npos) && (s.find("=") == s.rfind("="))) {
		//ASSIGN is passed as long as it contains 1 "="
		result = 1;
	} else {
		end = s.find(" ");
		if (end == string::npos) {
			result = 0;
		} else {
			string firstWord = s.substr(0, end);
			s = trim(s.substr(s.find(" "), s.size() - s.find(" ")));
			if (s.find(" ") == string::npos) {
				//READ, CALL, PRINT is passed as long as it contains 2 words
				if (firstWord.compare("call") == 0) result = 2;
				else if (firstWord.compare("read") == 0) result = 3;
				else if (firstWord.compare("print") == 0) result = 4;
			}
		}
	}
	return result;
}

/*
types: { "IF", "WHILE" } as of iter #1
validation of syntax errors
errors covered: incorrect syntax for if, while
errors NOT covered: incorrect variables, expression (i.e. *+/-=), non-alphanumeric characters
*/
int Preprocessor::validateCurvedBrackets(string s)
{
	int result = 0;
	if (s.substr(0, 5).compare("while") == 0) {
		s = trim(s.substr(5, s.size() - 5));
		if (s[0] == '(' && s[s.size() - 1] == ')') {
			result = 5;
		}
	}
	else if (s.substr(0, 2).compare("if") == 0) {
		//IF is passed as long as it is followed by '(' and ')' and last word is THEN
		if (s.substr(s.size() - 4, 4).compare("then") == 0) { // last word is then
			s = trim(s.substr(2, s.size() - 6));
			if (s[0] == '(' && s[s.size() - 1] == ')') result = 6;
		}
	}
	else if (s.substr(0, 4).compare("else") == 0) {
		if (trim(s).size() == 4) result = 7;
	}
	return result;
}

/*
types: { "PROCEDURE" } as of iter #1
validation of syntax errors
errors covered: incorrect syntax for procedure
errors NOT covered: incorrect variables, expression (i.e. *+/-=), non-alphanumeric characters
*/
int Preprocessor::validateProc(string s)
{
	int result = 0;
	string firstWord = s.substr(0, s.find(" "));
	if (firstWord.compare("procedure") == 0) {
		//PROCEDURE is passed as long as it contains 2 words
		s = trim(s.substr(9, s.size() - 9));
		if (s.find(" ") == string::npos) result = 8;
	}
	return result;
}

/*
trim whitespace from front and back of string
adapted from an answer in StackOverflow
https://stackoverflow.com/questions/44973435/stdptr-fun-replacement-for-c17/44973498#44973498
to save memory, will change this to void method in future
*/
string Preprocessor::trim(string s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
		return !isspace(ch);
	}));
	s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
		return !isspace(ch);
	}).base(), s.end());
	return s;
}
;
