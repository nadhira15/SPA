#include<stdio.h>
#include<string>
#include<vector>
#include<string>
#include<sstream>
#include<regex>
#include"ProcedureParser.h"

using namespace std;

//take in full statement string
std::string ProcedureParser:: parseProcName(const std::string toParse){
	std::smatch matches;
	std::regex reg("\\s*procedure\\s+([a-zA-Z]+[a-zA-Z]*\\d*)\\s*");
	if (std::regex_match(toParse, matches, reg)) {
		std::ssub_match procedureName = matches[1];
		std::string name = procedureName.str();
		return name;
	}
	else {
		return "failed";
	}
}

bool ProcedureParser::varifyProcName(const std::string toParse) {
	std::smatch matches;
	std::regex reg("\\s*procedure\\s+([a-zA-Z]+[a-zA-Z]*\\d*)\\s*");
	if (std::regex_match(toParse, matches, reg)) {
		std::ssub_match procedureName = matches[1];
		std::string name = procedureName.str();
		return true;
	}
	else {
		return false;
	}
}