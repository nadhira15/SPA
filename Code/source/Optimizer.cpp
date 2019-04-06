#include "Optimizer.h"

/* 
* Constructor function
*/
Optimizer::Optimizer(
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> st,
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> w,
	std::vector<std::pair<std::string, std::pair<std::string, std::string>>> p, 
	std::unordered_set<std::string> s, std::unordered_map<std::string, std::string> d) {
	suchThatClauses = st;
	withClauses = w;
	patternClauses = p;
	selectClause = s;
	declarations = d;
	//internal reference of clauses will follow: p, w, st
	psize = patternClauses.size();
	wsize = withClauses.size();
}

/*
* main processing function
*/
void Optimizer::groupClause() {
	//extract synonyms and create the syn-cl and cl-syn map
	int clauseSize = psize + wsize + suchThatClauses.size();
	for (int i = 0; i < clauseSize; i++) {
		groupByClauseType(i);
	}
	//while loop to chain clauses (DFS)
	std::pair<int,int> cl;
	bool isTrivial;
	while (!clSet.empty()) {
		//function for the more efficient clause? get from PKB statistics or use pq
		cl = *clSet.begin(); //rn use the first in set
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
void Optimizer::groupByClauseType(int i) {
	std::pair<int,std::vector<std::string>> prioritySyn;
	//clauses are represented internally by int, in the order pattern, with, st
	if (i > wsize + psize) {
		prioritySyn = extractSuchThatSyn(i - wsize - psize);
		createMaps(prioritySyn.second, std::make_pair(prioritySyn.first, i));
	} else if (i > psize) {
		prioritySyn = extractWithSyn(i - psize);
		createMaps(prioritySyn.second, std::make_pair(prioritySyn.first, i));
	} else {
		prioritySyn = extractPatternSyn(i);
		createMaps(prioritySyn.second, std::make_pair(prioritySyn.first, i));
	}
}

/*
* Creates syn-cl and cl-syn maps 
*/
void Optimizer::createMaps(std::vector<std::string> synLst, std::pair<int,int> cl) {
	clMap[cl.second] = synLst;
	clSet.insert(cl);
	for (std::vector<std::string>::iterator it = synLst.begin(); it != synLst.end(); ++it) {
		if (synMap.find(*it) == synMap.end()) {
			synMap[*it] = std::priority_queue<std::pair<int,int>>();
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
bool Optimizer::mapClauses(std::pair<int,int> cl, bool isTrivial) {
	std::pair<std::string, std::pair<std::string, std::string>> clause;
	clSet.erase(cl); //clause removed from bookkeeping sets
	for (std::vector<std::string>::iterator it = clMap[cl.second].begin(); it != clMap[cl.second].end(); ++it) {
		if (synSet.find(*it) != synSet.end()) { //check if synonym is already (partially) processed
			clause = getClause(cl.second);
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
bool Optimizer::mapSynonym(std::string syn, bool isTrivial) {
	//check if syn is part of select if group is trivial
	if (isTrivial && (selectClause.find(syn) != selectClause.end())) {
		isTrivial = false;
	}
	while (!synMap[syn].empty()) {
		std::pair<int,int> cl = synMap[syn].top();
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
std::pair<int, std::vector<std::string>> Optimizer::extractSuchThatSyn(int index) {
	std::vector<std::string> synLst;
	std::string r = suchThatClauses.at(index).first;
	std::string f = suchThatClauses.at(index).second.first;
	std::string s = suchThatClauses.at(index).second.second;
	int synNum = 0;
	int tmp = OptimizerUtility::getSuchThatEntityType(f);
	if (tmp == 1) {
		synLst.push_back(f);
	}
	if (tmp > 0) {
		synNum++;
	}
	tmp = OptimizerUtility::getSuchThatEntityType(s);
	if (tmp == 1) {
		synLst.push_back(s);
	}
	if (tmp > 0) {
		synNum++;
	}
	tmp = OptimizerUtility::getSuchThatPriority(r, synNum);
	return std::make_pair(tmp, synLst);
}

std::pair<int, std::vector<std::string>> Optimizer::extractPatternSyn(int index) {
	std::vector<std::string> synLst;
	std::string syn = patternClauses.at(index).first;
	std::string f = patternClauses.at(index).second.first;
	std::string s = patternClauses.at(index).second.second;
	synLst.push_back(syn);
	int tmp1 = OptimizerUtility::getPatternEntityType1(f);
	if (tmp1 == 1) {
		synLst.push_back(f);
	}
	//get from declaration --> syn type of syn
	std::string type = declarations[syn];
	int tmp2 = 0;
	if (type.compare("assign") == 0) {
		tmp2 = OptimizerUtility::getPatternEntityType2(s);
	}
	int p = OptimizerUtility::getPatternPriority(type, tmp1, tmp2);
	return std::make_pair(p, synLst);
}

std::pair<int, std::vector<std::string>> Optimizer::extractWithSyn(int index) {
	std::vector<std::string> synLst;
	std::string f = withClauses.at(index).second.first;
	std::string s = withClauses.at(index).second.second;
	int synNum = 0;
	if (OptimizerUtility::isWithSynonym(f)) {
		synLst.push_back(f.substr(0,f.find(".")));
		synNum++;
	}
	if (OptimizerUtility::isWithSynonym(s)) {
		synLst.push_back(s.substr(0, s.find(".")));
		synNum++;
	}
	int p = OptimizerUtility::getWithPriority(synNum);
	return std::make_pair(p, synLst);
}

/* Getter functions */
std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> Optimizer::getTrivial() {
	return trivial;
}
std::vector<std::vector<std::pair<std::string, std::pair<std::string, std::string>>>> Optimizer::getNonTrivial() {
	return nontrivial;
}

std::pair<std::string, std::pair<std::string, std::string>> Optimizer::getClause(int cl) {
	std::pair<std::string, std::pair<std::string, std::string>> clause;
	if (cl > psize + wsize) {
		clause = suchThatClauses.at(cl - psize - wsize);
	}
	else if (cl > psize) {
		clause = withClauses.at(cl - psize);
	}
	else {
		clause = patternClauses.at(cl);
	}
	return clause;
}