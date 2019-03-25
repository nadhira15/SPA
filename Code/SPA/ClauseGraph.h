#pragma once

#include <vector>
#include <string>


class ClauseGraph {
private:
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> clauseLst;
	std::vector<std::string> selectClause;
public:
	//constructor
	ClauseGraph(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> cl, std::vector<std::string> select);
	void GroupClause();
};