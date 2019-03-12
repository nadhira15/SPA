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
  (1) create preprocesser and get procedrue list: Preprocessor(string chunk).getProcLst();
*/

class Preprocessor {
private:
	string chunk;
	int size;
	int stopper;
	int stmtNum = 1;
	vector<Statement> procLst;
	stack<int> ifStmt;
	Statement processProc(int bookmark);
	vector<Statement> processLst(int bookmark);
public:
	//only validators, atring trimmer, getter and Processer is public
	int validateSemicolon(string s);
	int validateCurvedBrackets(string s);
	int validateProc(string s);
	string trim(string s);
	vector<Statement> getProcLst();
	Preprocessor(string chunk1);
	void process();
};

