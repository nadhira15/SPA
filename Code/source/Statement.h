#pragma once

#include <vector>
#include <string>
#include <iostream>

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

class Statement {
private:
	std::string extracted_String;
	std::vector<Statement> stmtLst;
	int stmtType;
	int stmtNum;
public:
	//constructors
	Statement(std::string s, int type, int stmtNum);
	Statement(std::string s, std::vector<Statement> lst, int type, int stmtNum);
	//getters
	std::string getString();
	std::vector<Statement> getStmtLst();
	int getType(); // 1 "ASSIGN", 2 "CALL", 3 "READ", 4 "PRINT", 5 "WHILE", 6 "IF", 7 "ELSE", 8 "PROCEDURE"
	int getStmtNum();
	bool hasStmtLst();
	//print (for troubleshooting)
	void printStatement();
	void print();
};
