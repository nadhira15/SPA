#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Statement.h"

using namespace std;
//string typeLst[]  = { 1 "ASSIGN", 2 "CALL", 3 "READ", 4 "PRINT", 6 "IF", 5 "WHILE", 7 "PROCEDURE" }; //as of iter #1

//constructors
Statement::Statement() {}
Statement::Statement(string s, int type) { //constructor for ASSIGN, CALL, READ, PRINT
	extracted_String = s;
	stmtType = type;
}
Statement::Statement(string s, vector<Statement> lst, int type) { //constructor for IF, WHILE 
	extracted_String = s;
	stmtLst = lst;
	stmtType = type;
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
bool Statement::hasStmtLst() {
	return stmtType > 4;
}
void Statement::setString(string s) {
	extracted_String = s;
}
void Statement::printStatement() {
	cout << "<" << getString() << ">\n";
}
void Statement::print() {
	printStatement();
	if (hasStmtLst()) {
		for (int i = 0; i < getStmtLst().size(); i++) {
			getStmtLst().at(i).print();
		}
	}
}
