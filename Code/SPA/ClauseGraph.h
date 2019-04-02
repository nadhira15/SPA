#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class ClauseGraph {
private:
	enum clauseType { st, w, p }; //clauses are represented by <clauseType,index>
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatClauses;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> withClauses;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternClauses;
	std::unordered_set<std::string> selectClause;
	std::unordered_map<std::string, std::vector<std::pair<clauseType,int>>> synMap;
	std::unordered_map<std::pair<clauseType, int>, std::vector<std::string>> clMap;
	std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> trivial;
	std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> nontrivial;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> graph;
	std::unordered_set<std::string> synSet;
	std::unordered_set<std::pair<clauseType,int>> clSet;
	void groupByClauseType(clauseType t);
	std::vector<std::string> extractSuchThatSyn(int index);
	std::vector<std::string> extractWithSyn(int index);
	std::vector<std::string> extractPatternSyn(int index);
	void createMaps(std::vector<std::string> synLst, std::pair<clauseType, int> cl);
	bool mapClauses(std::pair<clauseType, int> cl, bool trivial);
	bool mapSynonym(std::string syn, bool trivial);
public:
	//constructor
	ClauseGraph(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> w,
		std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p,
		std::unordered_set<std::string> select);
	void groupClause();
	std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> getTrivial();
	std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> getNonTrivial();
};