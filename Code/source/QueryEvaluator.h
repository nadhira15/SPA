#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"

class QueryEvaluator
{
public:
	static std::unordered_set<std::string> projectResult(
		std::unordered_map<std::string, std::string> declarations,
		std::vector<std::string> selectedVar,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatCondition,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternCondition,
		std::vector<pair<std::string, std::string>> withCondition);
	static std::pair<std::string, std::unordered_map<std::string, std::vector<std::string>>> evaluateTable(
		std::unordered_map<std::string, std::string> declarations,
		vector<pair<string, pair<string, string>>> suchThatCondition,
		vector<pair<string, pair<string, string>>> patternCondition,
		std::vector<std::pair<std::string, std::string>> withCondition);

	static std::unordered_map<std::string, std::vector<std::string>> evaluatePatternCondition(
		std::unordered_map<std::string, std::string> declarations,
		std::pair<std::string, std::pair<std::string, std::string>> pattern);

	static string isSuchThatTrivial(string relation, string firstArgument, string secondArgument);
	static std::unordered_map<std::string, std::vector<std::string>> evaluateSuchThat(
		std::unordered_map<std::string, std::string> declarations,
		string relation, string firstArgument, string secondArgument);

	static std::unordered_set<std::string> getAllStms();
	static std::unordered_map<std::string, std::vector<std::string>> getStmts(
		std::unordered_map<std::string, std::string> declarations, 
		string syn);
	static std::unordered_map<std::string, std::vector<std::string>> filterType(string synonym,
		std::unordered_map<std::string, std::string> declarations,
		std::unordered_map<std::string, std::vector<std::string>> toBeFiltered);
	static std::unordered_map<std::string, std::vector<std::string>> filterType(string synonym1, 
		string synonym2, std::unordered_map<std::string, std::string> declarations,
		std::unordered_map<std::string, std::vector<std::string>> toBeFiltered);

	static string truthValue(bool boolean);
	static string trimFrontEnd(string quotedString);

	static bool isInteger(string s);
	static bool isQuoted(string s);
	static bool isSynonym(string s);

};