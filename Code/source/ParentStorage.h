#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

#include "Hasher.h"

/*
	A structure to contain:	The statement's Parent,
							a list of it's children,
							a list of ancestors including the direct parent,
							a list of descendants including its' direct children
	If such statement or list does not exist, it'll be set to 0 or {} respectively
*/
struct pRelationships
{
	int parent;
	unordered_set<int> children;
	unordered_set<int> ancestors;
	unordered_set<int> descendants;
};

/*
	A storage class to store the Parent & Parent* relationships of statements
*/
class ParentStorage {
public:
	ParentStorage();

	bool addParent_Child(int parent, int child);
	bool setAncestors(int descendant, unordered_set<int> ancestors);
	bool setDescendants(int ancestor, unordered_set<int> descendants);

	bool isEmpty();
	bool isParent(int stm);
	bool isChild(int stm);
	bool hasAncDescPair(pair<int, int> pair);

	int getParent(int stm);
	unordered_set<int> getChildren(int stm);
	unordered_set<int> getAncestors(int stm);
	unordered_set<int> getDescendants(int stm);
	unordered_set<int> getAllParent();
	unordered_set<int> getAllChildren();
	unordered_set< pair<int, int>, intPairhash> getParentChildPairs();
	unordered_set< pair<int, int>, intPairhash> getAncDescPair();

private:
	static unordered_map<int, pRelationships> parentTable;
	static unordered_set< pair<int, int>, intPairhash> parent_ChildPairList;
	static unordered_set< pair<int, int>, intPairhash> anc_DescPairList;
	static unordered_set<int> parentList;
	static unordered_set<int> childrenList;
};