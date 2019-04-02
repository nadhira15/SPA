#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

/** Usage Guide
  This class optimises the evaluator by chaining the clauses together such that every other clause will have at least 1 clause evaluated
  
  // To use:
  ClauseGraph cg = ClauseGraph(st, w, p, s);
  cg.groupClause();
  
  // To get the groups of trivial/non-trivial clauses:
  std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> trivial = cg.getTrivial();
  std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> nontrivial = cg.getNonTrivial();
*/

class ClauseGraph {
private:
	// these variables will be used as reference and will NOT be altered after creation
	enum clauseType { st, w, p };
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatClauses;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> withClauses;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternClauses;
	std::unordered_set<std::string> selectClause;
	std::unordered_map<std::string, std::vector<std::pair<clauseType,int>>> synMap;
	std::unordered_map<std::pair<clauseType, int>, std::vector<std::string>> clMap;
	// these variables are used for storage of results
	std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> trivial;
	std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> nontrivial;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> graph;
	// sets are used for bookkeeping to prevent infinite calls
	std::unordered_set<std::string> synSet;
	std::unordered_set<std::pair<clauseType,int>> clSet;
	// processing functions - refer to .cpp for more details
	void groupByClauseType(clauseType t);
	std::vector<std::string> extractSuchThatSyn(int index);
	std::vector<std::string> extractWithSyn(int index);
	std::vector<std::string> extractPatternSyn(int index);
	void createMaps(std::vector<std::string> synLst, std::pair<clauseType, int> cl);
	bool mapClauses(std::pair<clauseType, int> cl, bool trivial);
	bool mapSynonym(std::string syn, bool trivial);
public:
	//Constructor
	ClauseGraph(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> w,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p,
		std::unordered_set<std::string> select);
	//Statement to process
	void groupClause();
	//Getter Functions
	std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> getTrivial();
	std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> getNonTrivial();
};