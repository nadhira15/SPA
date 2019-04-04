#include "ClauseGraph.h"

/* 
* Constructor function
*/
ClauseGraph::ClauseGraph(
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st,
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> w,
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p, 
	std::unordered_set<std::string> select) {
	suchThatClauses = st;
	withClauses = w;
	patternClauses = p;
	selectClause = select;

}

/*
* main processing function
*/
void ClauseGraph::groupClause() {
	//extract synonyms and create the syn-cl and cl-syn map
	groupByClauseType(0);
	groupByClauseType(1);
	groupByClauseType(2);
	//while loop to chain clauses (DFS)
	std::tuple<int, int, int> cl;
	bool isTrivial;
	while (!clSet.empty()) {
		//function for the more efficient clause? get from PKB statistics or use pq
		cl = *clSet.begin(); //rn use the first in unordered set
		isTrivial = mapClauses(cl, true);
		if (isTrivial) {
			trivial.push_back(graph);
		} else {
			nontrivial.push_back(graph);
		}
		graph.clear();
	}
}

/*
* Creates a hashmap of syn-clause and clause-syn, which will be used for graph traversal
*/
void ClauseGraph::groupByClauseType(int t) {
	std::pair<int,std::vector<std::string>> prioritySyn;
	//clauses are represented internally by <int,index>
	if (t == 0) {
		for (int i = 0; i < suchThatClauses.size(); i++) {
			prioritySyn = extractSuchThatSyn(i);
			createMaps(prioritySyn.second, std::make_tuple(prioritySyn.first, t, i));
		}
	} else if (t == 1) {
		for (int i = 0; i < patternClauses.size(); i++) {
			prioritySyn = extractPatternSyn(i);
			createMaps(prioritySyn.second, std::make_tuple(prioritySyn.first, t, i));
		}
	} else {
		for (int i = 0; i < withClauses.size(); i++) {
			prioritySyn = extractWithSyn(i);
			createMaps(prioritySyn.second, std::make_tuple(prioritySyn.first, t, i));
		}
	}
}

/*
* Creates syn-cl and cl-syn maps 
*/
void ClauseGraph::createMaps(std::vector<std::string> synLst, std::tuple<int, int, int> cl) {
	clMap[cl] = synLst;
	clSet.insert(cl);
	for (std::vector<std::string>::iterator it = synLst.begin(); it != synLst.end(); ++it) {
		if (synMap.find(*it) == synMap.end()) {
			synMap[*it] = std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, Compare>();
		} else {
			synMap[*it].push(cl);
			synSet.insert(*it);
		}

	}
}

/* 
* Chains synonyms and clauses together
* Input: clause, isTrivial
* Output: isTrivial
* Bookeeping: as this is DFS, a synSet and clSet is used for bookkeeping
  i.e. once a clause/synonym is accessed, no other synonym/clause can access it
  As the for loop is implemented, the algorithm is guaranteed to access all synonyms of the clause and vice versa
* Note: The actual clause, rather than the internal reference, is added to the results graph
*/
bool ClauseGraph::mapClauses(std::tuple<int, int, int> cl, bool isTrivial) {
	std::pair<std::string, std::pair<std::string, std::string>> clause;
	clSet.erase(cl); //clause removed from bookkeeping sets
	for (std::vector<std::string>::iterator it = clMap[cl].begin(); it != clMap[cl].end(); ++it) {
		if (synSet.find(*it) != synSet.end()) { //check if synonym is already (partially) processed
			clause = getClause(cl);
			graph.push_back(clause);
			isTrivial = mapSynonym(*it, isTrivial);
		}
	}
	return isTrivial;
}

/*
* Chains synonyms and clauses together
* Input: synonym, isTrivial
* Output: isTrivial
* Bookeeping: as this is DFS, a synSet and clSet is used for bookkeeping
  i.e. once a clause/synonym is accessed, no other synonym/clause can access it
  As the for loop is implemented, the algorithm is guaranteed to access all synonyms of the clause and vice versa
* Note: Group is assumed to be trivial until a select synonym matches
*/
bool ClauseGraph::mapSynonym(std::string syn, bool isTrivial) {
	//check if syn is part of select if group is trivial
	if (isTrivial && (selectClause.find(syn) != selectClause.end())) {
		isTrivial = false;
	}
	while (!synMap[syn].empty()) {
		std::tuple<int, int, int> cl = synMap[syn].top();
		synMap[syn].pop();
		if (clSet.find(cl) != clSet.end()) { //check if clause is already processed
			isTrivial = mapClauses(cl, isTrivial);
		}
	}
	synSet.erase(syn); //synonym removed from bookkeeping sets
	return isTrivial;
}

/*
* To extract synonym from the such that, pattern, with clauses
*/
std::pair<int, std::vector<std::string>> ClauseGraph::extractSuchThatSyn(int index) {
	std::vector<std::string> synLst;
	std::string r = suchThatClauses.at(index).first;
	std::string f = suchThatClauses.at(index).second.first;
	std::string s = suchThatClauses.at(index).second.second;
	int synNum = 0;
	int tmp = getSuchThatEntityType(f);
	if (tmp == 1) {
		synLst.push_back(f);
	}
	if (tmp > 0) {
		synNum++;
	}
	tmp = getSuchThatEntityType(s);
	if (tmp == 1) {
		synLst.push_back(s);
	}
	if (tmp > 0) {
		synNum++;
	}
	tmp = getSuchThatPriority(r, synNum);
	return std::make_pair(tmp, synLst);
}

std::pair<int, std::vector<std::string>> ClauseGraph::extractPatternSyn(int index) {
	std::vector<std::string> synLst;
	std::string syn = patternClauses.at(index).first;
	std::string f = patternClauses.at(index).second.first;
	std::string s = patternClauses.at(index).second.second;
	synLst.push_back(syn);
	int tmp1 = getPatternEntityType1(f);
	if (tmp1 == 1) {
		synLst.push_back(f);
	}
	//get from declaration --> syn type of syn
	std::string type;
	int tmp2 = 0;
	if (type.compare("assign") == 0) {
		tmp2 = getPatternEntityType2(s);
	}
	int p = getPatternPriority(type, tmp1, tmp2);
	return std::make_pair(p, synLst);
}

std::pair<int, std::vector<std::string>> ClauseGraph::extractWithSyn(int index) {
	std::vector<std::string> synLst;
	std::string f = withClauses.at(index).second.first;
	std::string s = withClauses.at(index).second.second;
	int synNum = 0;
	if (isWithSynonym(f)) {
		synLst.push_back(f);
		synNum++;
	}
	if (isWithSynonym(s)) {
		synLst.push_back(s);
		synNum++;
	}
	int p = getWithPriority(synNum);
	return std::make_pair(p, synLst);
}

/* Getter functions */
std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> ClauseGraph::getTrivial() {
	return trivial;
}
std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> ClauseGraph::getNonTrivial() {
	return nontrivial;
}

std::pair<std::string, std::pair<std::string, std::string>> ClauseGraph::getClause(std::tuple<int, int, int> cl) {
	std::pair<std::string, std::pair<std::string, std::string>> clause;
	if (std::get<1>(cl) == 0) {
		clause = suchThatClauses.at(std::get<2>(cl));
	}
	else if (std::get<1>(cl) == 1) {
		clause = patternClauses.at(std::get<2>(cl));
	}
	else {
		clause = withClauses.at(std::get<2>(cl));
	}
	return clause;
}

bool Compare::operator()(std::tuple<int, int, int> c1, std::tuple<int, int, int> c2) {
	return std::get<0>(c1) < std::get<0>(c2);
}