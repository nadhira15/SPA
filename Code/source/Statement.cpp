#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Statement.h"

using namespace std;
// types = { 1 "ASSIGN", 2 "CALL", 3 "READ", 4 "PRINT", 5 "WHILE", 6 "IF", 7 "ELSE", 8 "PROCEDURE" }; //as of iter #1

//constructors
Statement::Statement() {}
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
void Statement::setString(string s) {
	extracted_String = s;
}
void Statement::printStatement() {
	cout << getStmtNum() << "   <" << getString() << ">" << getType() << "\n";
}
void Statement::print() {
	printStatement();
	if (hasStmtLst()) {
		for (size_t i = 0; i < getStmtLst().size(); i++) {
			getStmtLst().at(i).print();
		}
	}
}
