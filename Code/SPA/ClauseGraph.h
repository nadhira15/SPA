#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class ClauseGraph {
private:
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> suchThatClauses;
	std::vector<std::pair<std::string, std::string>> withClauses;
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> patternClauses;
	std::vector<std::string> selectClause;
	std::unordered_map<std::string, std::vector<std::pair<std::string,int>>> synMap;
	std::unordered_map<std::pair<std::string, int>, std::unordered_set<std::pair<std::string,int>> *> clMap;
	void groupByClauseType(std::string str);
	std::vector<std::string> ClauseGraph::extractSuchThatSyn(int index);
	std::vector<std::string> ClauseGraph::extractWithSyn(int index);
	std::vector<std::string> ClauseGraph::extractPatternSyn(int index);
	void mapSynonyms(std::vector<std::string> synLst, std::pair<std::string, int> cl);
public:
	//constructor
	ClauseGraph(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st,
		 std::vector<std::pair<std::string, std::string>> w, 
		 std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p, 
		 std::vector<std::string> select);
	void groupClause();
	std::vector<std::string> extractSyn(int cl);
};