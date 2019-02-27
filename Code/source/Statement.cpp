#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Statement.h"

using namespace std;

/**** Usage guide
  Statement is a container for the processed source code
  Each Statement will contain:
  (1) string extracted_string representing the code
  (2) vector<Statement> stmtLst representing the list of statements for if/else/while statements
  (3) int stmtType representing the type of statement as of iter #1:
	  types = { 1 "ASSIGN", 2 "CALL", 3 "READ", 4 "PRINT", 5 "WHILE", 6 "IF", 7 "ELSE", 8 "PROCEDURE" };
  (4) int stmtNum representing the statement number
	  *NOTE statement number of else is the same as the if statement number it belongs to

  Statement methods:
  (1) constructors:
  Statement(string s, int type, int stmtNum): for assign, print, call, read statements
  Statement(string s, vector<Statement> lst, int type, int stmtNum): for if, else, while statements

  (2) getters:
  getString() returns string
  getStmtLst() returns its statement list
  int getType() returns integer representing type (1 "ASSIGN", 2 "CALL", 3 "READ", 4 "PRINT", 5 "WHILE", 6 "IF", 7 "ELSE", 8 "PROCEDURE")
  int getStmtNum() returns integer representing statement number
  bool hasStmtLst() returns true if statement has a statement list, false otherwise
	*NOTE will return true for if, else, while even if its statement list is empty
*/

//constructors
Statement::Statement(string s, int type, int sNum) { //constructor for ASSIGN, CALL, READ, PRINT
	extracted_String = s;
	stmtType = type;
	stmtNum = sNum;
}
Statement::Statement(string s, vector<Statement> lst, int type, int sNum) { //constructor for IF, WHILE 
	extracted_String = s;
	stmtLst = lst;
	stmtType = type;
	stmtNum = sNum;
}
//getters
string Statement::getString() {
	return extracted_String;
}
vector<Statement> Statement::getStmtLst() {
	return stmtLst;
}
int Statement::getType() {
	return stmtType;
}
int Statement::getStmtNum() {
	return stmtNum;
}
bool Statement::hasStmtLst() {
	return stmtType > 4;
}
//for troubleshooting
void Statement::printStatement() {
	cout << getStmtNum() << "   <" << getString() << ">" << getType() << "\n";
}
void Statement::print() {
	printStatement();
	if (hasStmtLst()) {
		for (int i = 0; i < getStmtLst().size(); i++) {
			getStmtLst().at(i).print();
		}
	}
}
