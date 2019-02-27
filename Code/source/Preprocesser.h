#pragma once

#include <vector>
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

#include "Statement.h"

using namespace std;

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

class Preprocesser {
private:
	string chunk;
	int count1 = 0;
	int count2 = 0;
	int stopper;
	int stmtNum = 0;
	vector<Statement> procLst;
	stack<int> ifStmt;
	void error(int i);
	Statement processProc(int bookmark, int last);
	vector<Statement> processLst(int bookmark, int last);
	vector<string> KEYWORDS;
public:
	//only validators, atring trimmer, getter and Processer is public
	int validateSemicolon(string s);
	int validateCurvedBrackets(string s);
	int validateProc(string s);
	string trim(string s);
	vector<Statement> getProcLst();
	Preprocesser(string chunk1);
};

