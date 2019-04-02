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
	groupByClauseType(st);
	groupByClauseType(w);
	groupByClauseType(p);
	//while loop to chain clauses (DFS)
	std::pair<clauseType, int> cl;
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
void ClauseGraph::groupByClauseType(clauseType t) {
	std::vector<std::string> synLst;
	//clauses are represented internally by <clauseType,index>
	if (t == st) {
		for (int i = 0; i < suchThatClauses.size(); i++) {
			synLst = extractSuchThatSyn(i);
			createMaps(synLst, std::make_pair(st, i));
		}
	} else if (t == p) {
		for (int i = 0; i < patternClauses.size(); i++) {
			synLst = extractPatternSyn(i);
			createMaps(synLst, std::make_pair(p, i));
		}
	} else {
		for (int i = 0; i < withClauses.size(); i++) {
			synLst = extractWithSyn(i);
			createMaps(synLst, std::make_pair(w, i));
		}
	}
}

/*
* Creates syn-cl and cl-syn maps 
*/
void ClauseGraph::createMaps(std::vector<std::string> synLst, std::pair<clauseType, int> cl) {
	clMap[cl] = synLst;
	for (std::vector<std::string>::iterator it = synLst.begin(); it != synLst.end(); ++it) {
		synMap[*it].push_back(cl);
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
bool ClauseGraph::mapClauses(std::pair<clauseType, int> cl, bool isTrivial) {
	std::pair<std::string, std::pair<std::string, std::string>> clause;
	clSet.erase(cl); //clause removed from bookkeeping sets
	for (std::vector<std::string>::iterator it = clMap[cl].begin(); it != clMap[cl].end(); ++it) {
		if (synSet.find(*it) != synSet.end()) { //check if synonym is already (partially) processed
			if (cl.first == st) {
				clause = suchThatClauses.at(cl.second);
			} else if (cl.first == p) {
				clause = patternClauses.at(cl.second);
			} else {
				clause = withClauses.at(cl.second);
			}
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
	synSet.erase(syn); //synonym removed from bookkeeping sets
	for (std::vector<std::pair<clauseType, int>>::iterator it = synMap[syn].begin(); it != synMap[syn].end(); ++it) {
		if (clSet.find(*it) != clSet.end()) { //check if clause is already processed
			isTrivial = mapClauses(*it, isTrivial);
		}
	}
	return isTrivial;
}

/*
* To extract synonym from the such that, pattern, with clauses
*/
std::vector<std::string> ClauseGraph::extractSuchThatSyn(int index) {
	std::vector<std::string> synLst;
	std::string f = suchThatClauses.at(index).second.first;
	std::string s = suchThatClauses.at(index).second.second;
	if (isSuchThatSynonym(f)) synLst.push_back(f);
	if (isSuchThatSynonym(s)) synLst.push_back(s);
	return synLst;
}

std::vector<std::string> ClauseGraph::extractPatternSyn(int index) {
	std::vector<std::string> synLst;
	std::string f = patternClauses.at(index).first;
	std::string s = patternClauses.at(index).second.first;
	if (isPatternSynonym(f)) synLst.push_back(f);
	if (isPatternSynonym(s)) synLst.push_back(s);
	return synLst;
}

std::vector<std::string> ClauseGraph::extractWithSyn(int index) {
	std::vector<std::string> synLst;
	std::string f = withClauses.at(index).second.first;
	std::string s = withClauses.at(index).second.second;
	if (isWithSynonym(f)) synLst.push_back(f);
	if (isWithSynonym(s)) synLst.push_back(s);
	return synLst;
}

/* Getter functions */
std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> ClauseGraph::getTrivial() {
	return trivial;
}
std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> ClauseGraph::getNonTrivial() {
	return nontrivial;
}
