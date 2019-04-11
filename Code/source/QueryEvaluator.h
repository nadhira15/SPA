#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Optimizer.h"
#include "PKB.h"

class QueryEvaluator
{
public:
	static std::list<std::string> projectResult(
		std::unordered_map<std::string, std::string> declarations,
		std::vector<std::string> selectedVar,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatCondition,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternCondition,
		std::vector<std::pair<std::string, std::string>> withCondition);

private:
	static std::list<std::string> toStringList(
		std::unordered_map<std::string, std::string> declarations,
		std::vector<std::string> selectedVar,
		std::unordered_map<std::string, std::vector<std::string>> projectTable);

	static std::unordered_map<std::string, std::vector<std::string>> getGroupProjectTable(
		std::unordered_map<std::string, std::string> declarations,
		std::vector<std::string> selectedVar,
		std::unordered_map<std::string, std::vector<std::string>> groupTable);

	static std::unordered_map<std::string, std::vector<std::string>> evaluateGroup(
		std::unordered_map<std::string, std::string> declarations,
		std::vector<std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>>>
			clauseGroup);

	static std::string isWithTrivial(std::string left, std::string right);

	static std::unordered_map<std::string, std::vector<std::string>>
		evaluateWithPair(std::unordered_map<std::string, std::string> declarations,
						 std::string attr);

	static std::unordered_map<std::string, std::vector<std::string>>
		evaluateWithCondition(std::unordered_map<std::string,std::string> declarations,
			std::string firstArgument, std::string secondArgument);

	static std::unordered_map<std::string, std::vector<std::string>>
		evaluatePatternCondition(std::unordered_map<std::string, std::string> declarations,
			std::string patternSynonym, std::string firstArgument, std::string secondArgument);

	static std::string isSuchThatTrivial(std::string relation, std::string firstArgument,
										 std::string secondArgument);

	static std::unordered_map<std::string, std::vector<std::string>>
		evaluateSuchThat(std::unordered_map<std::string, std::string> declarations,
						 std::string relation, std::string firstArgument, std::string secondArgument);

	static std::unordered_set<std::string> getAllStms();

	static std::unordered_map<std::string, std::vector<std::string>>
		getStmtsMap(std::unordered_map<std::string, std::string> declarations, std::string syn);

	static std::unordered_set<std::string> getStmts(std::string);

	static std::unordered_map<std::string, std::vector<std::string>>
		filterType(std::string synonym, std::unordered_map<std::string, std::string> declarations,
				   std::unordered_map<std::string, std::vector<std::string>> toBeFiltered);

	static std::unordered_map<std::string, std::vector<std::string>>
		filterType(std::string synonym1, std::string synonym2,
				   std::unordered_map<std::string, std::string> declarations,
				   std::unordered_map<std::string, std::vector<std::string>> toBeFiltered);

	static std::string toAttrRefVal(std::unordered_map<std::string, std::string> declarations,
									std::string attrRef, std::string attrMember);

	static std::string truthValue(bool boolean);

	static std::string trimFrontEnd(std::string quotedString);

	static bool isInteger(std::string s);
	static bool isQuoted(std::string s);
	static bool isSynonym(std::string s);
	static bool hasReference(std::string s);
	static std::string attrOf(std::string s);
	static std::string refOf(std::string s);

};