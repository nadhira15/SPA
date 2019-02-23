#pragma once

#include <vector>
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Statement.h"

using namespace std;
Statement processProc(int bookmark, int last);
vector<Statement> processLst(int bookmark, int last);
int validateSemicolon(string s);
int validateCurvedBrackets(string s);
int validateProc(string s);
string trim(string s);
vector<Statement> getProcLst();
void Processer(string chunk1);
