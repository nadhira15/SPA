#pragma once
#include<stdio.h>
#include<regex>

#include "LexicalToken.h"

class CallParser
{
public:
	std::string parseCallStmt(std::string toParse);
private:
	bool verifyCallStmt(std::string statement);
};
