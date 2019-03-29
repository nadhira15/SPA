#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class ClauseGraph {
private:
	enum clauseType { st, w, p };
	int counter = 0;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatClauses;
	std::vector<std::pair<std::string, std::string>> withClauses;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternClauses;
	std::vector<std::string> selectClause;
	std::unordered_map<std::string, std::vector<std::pair<clauseType,int>>> synMap;
	std::unordered_map<std::pair<clauseType, int>, std::vector<int>> clMap;
	std::unordered_map<int, std::unordered_set<std::pair<std::string, std::pair<std::string, std::string>>>> stMap;
	std::unordered_map<int, std::unordered_set<std::pair<std::string, std::string>>> wMap;
	std::unordered_map<int, std::unordered_set<std::pair<std::string, std::pair<std::string, std::string>>>> pMap;
	void groupByClauseType(clauseType t);
	std::vector<std::string> ClauseGraph::extractSuchThatSyn(int index);
	std::vector<std::string> ClauseGraph::extractWithSyn(int index);
	std::vector<std::string> ClauseGraph::extractPatternSyn(int index);
	void mapSynonyms(std::vector<std::string> synLst, std::pair<clauseType, int> cl);
	void mapClauses(std::vector<std::string> synLst, std::pair<clauseType, int> cl);
public:
	//constructor
	ClauseGraph(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st,
		 std::vector<std::pair<std::string, std::string>> w, 
		 std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p, 
		 std::vector<std::string> select);
	void groupClause();
};