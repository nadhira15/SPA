#include "ClauseGraph.h"

ClauseGraph::ClauseGraph(std::vector<std::pair<std::string, std::pair<std::string, std::string>>> cl, std::vector<std::string> select) {
	clauseLst = cl;
	selectClause = select;
}


