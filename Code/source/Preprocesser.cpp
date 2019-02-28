#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

#include "Statement.h"
#include "Preprocesser.h"

/**** Usage guide
  Preprocesser will contain:
  (1) validate(...): the validator methods to validate statement types (used mainly for testing)
  (2) getProcLst: getter method for the procedure list
  (3) Preprocesser: constructor method to satrt the processing of source code

  Preprocesser:
  input: string representing source code
  output: the object

  getter methods: getProcLst
  input: null
  output: vector<Statement> representing the vector of procedures in the program

  to use:
  (1) create preprocesser and get procedrue list: Preprocesser(string chunk).getProcLst();
*/

vector<Statement> Preprocesser::getProcLst() {
	return procLst;
}

void Preprocesser::error(int t) {
	string s1 = "Pre-processing error, incorrect syntax for ";
	string s2 = (t == 0) ? "procedure" : (t == 1) ? "if/else, while" : "assign, call, print, read";
	//throw "\n" + s1 + s2 + "\n";
}

/* constructor
input: nil
output: vector<Statement> that contains procedure statements
*/
Preprocesser::Preprocesser(string chunk1)
{
	//take file contents and save as global variable
	chunk = chunk1;
	procLst.push_back(processProc(0, chunk.size()));
	/* 
	//bug: if variable name is procedure it will mess up the program
	size_t prev = chunk.find("procedure "), next;
	while (prev != string::npos) {
		next = chunk.find("procedure ", prev + 10);
		if (next == string::npos) {
			procLst.push_back(processProc(prev, chunk.size()));
		}
		else {
			procLst.push_back(processProc(prev, next));
		}
		prev = next;
	}
	*/
}
/*
input: start, end (not inclusive)
output: Statement
NOTE 'procedure' can only occur once per procedure
*/
Statement Preprocesser::processProc(int bookmark, int last)
{
	int valid;
	string tmp;
	size_t tmpn, pos;
	int tmpn2;
	//reset counters for '{' and '}'
	count1 = 1;
	count2 = 0;
	//validation: check for last '}' and pass validation to validateProc
	pos = chunk.find_last_not_of(" \t\f\v\n\r", last - 1);
	tmpn = chunk.find('{', bookmark);
	if (chunk[pos] != '}') {
		valid = 0;
		error(0);
		//!!error handling here
	} else if (tmpn != string::npos) {
		tmp = trim(chunk.substr(bookmark, tmpn - bookmark));
		valid = validateProc(tmp); //validate procedure term
	}
	if (valid == 0) {
		error(0);
	}
	tmpn2 = stmtNum;
	vector<Statement> stmtlst = processLst(tmpn + 1, pos);
	if (stmtlst.empty() || count1 != count2 || !ifStmt.empty()) {
		error(0);
	}
	Statement s = Statement(tmp, stmtlst, valid, tmpn2); //will proceed even with errors
	return s;
}

/*
input: start, end (usually })
output: Statement
*/
vector<Statement> Preprocesser::processLst(int bookmark, int last) {
	vector<Statement> stmtlst;
	int valid, i, tmpN;
	string tmp, tmp2;
	stopper = 0;
	for (i = bookmark; i < last; i++) {
		if (chunk[i] == '}') { //end of a {} part
			//double check count of '{' and '}'
			count2++;
			//reset stopper
			stopper = i + 1;
			//end loop by i = last
			i = last;
		}
		else if (chunk[i] == '{') {
			//double check count of '{' and '}'
			count1++;
			//return trimmed string of statement
			tmp = trim(chunk.substr(bookmark, i - bookmark));
			//check validity
			valid = validateCurvedBrackets(tmp);
			//based on results of validation
			if (valid == 0) {
				error(1);
			} else if (valid == 7) {
				// ELSE statement
				if (ifStmt.empty()) {
					error(1);
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
			stmtlst.push_back(Statement(tmp, processLst(i + 1, last), valid, tmpN));
			//after execution of previous process {}, return to previous bookmark and i-count
			i = stopper;
			bookmark = stopper;
		}
		else if (chunk[i] == ';') {
			tmp = trim(chunk.substr(bookmark, i - bookmark));
			bookmark = i + 1;
			tmpN = stmtNum;
			valid = validateSemicolon(tmp);
			if (valid == 0) {
				//ignore this statement
				error(2);
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
int Preprocesser::validateSemicolon(string s)
{
	int result = 0;
	size_t end;
	if ((s.find("=") != string::npos) && (s.find("=") == s.rfind("="))) {
		//ASSIGN is passed as long as it contains an "="
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
int Preprocesser::validateCurvedBrackets(string s)
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
int Preprocesser::validateProc(string s)
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
string Preprocesser::trim(string s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
		return !isspace(ch);
	}));
	s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
		return !isspace(ch);
	}).base(), s.end());
	return s;
}
;
