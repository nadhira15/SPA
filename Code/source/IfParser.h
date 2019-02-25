#pragma once

#include <vector>
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Statement.h"

using namespace std;

class IfParser {
public:
	string ifParse(short stmtNo, std::string stmt, vector<Statement> stmtlst);

};

