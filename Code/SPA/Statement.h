#pragma once

#include <vector>
using namespace std;

class Statement {
private:
	string extracted_String;
	vector<Statement> stmtLst;
	int stmtType;
	int stmtNum;
public:
	//constructors
	Statement();
	Statement(string s, int type, int stmtNum);
	Statement(string s, vector<Statement> lst, int type, int stmtNum);
	//getters
	string getString();
	vector<Statement> getStmtLst();
	int getType(); // 1 "ASSIGN", 2 "CALL", 3 "READ", 4 "PRINT", 5 "WHILE", 6 "IF", 7 "ELSE", 8 "PROCEDURE"
	int getStmtNum();
	bool hasStmtLst();
	//setter
	void setString(string s);
	//print (for troubleshooting)
	void printStatement();
	void print();
};
