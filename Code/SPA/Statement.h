#pragma once

#include <vector>
using namespace std;

class Statement {
private:
	string extracted_String;
	vector<Statement> stmtLst;
	int stmtType;
public:
	//constructors
	Statement();
	Statement(string s, int type);
	Statement(string s, vector<Statement> lst, int type);
	//getters
	string getString();
	vector<Statement> getStmtLst();
	int getType();
	bool hasStmtLst();
	//setter
	void setString(string s);
	void printStatement();
	void print();
};
