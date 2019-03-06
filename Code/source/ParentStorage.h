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
	bool containsAnc_Desc(pair<int, int> pair);

	int getParentOf(int stm);
	unordered_set<int> getChildrenOf(int stm);
	unordered_set<int> getAncestorsOf(int stm);
	unordered_set<int> getDescendantsOf(int stm);
	unordered_set<int> getParentList();
	unordered_set<int> getChildrenList();
	unordered_set< pair<int, int>, intPairhash> getParent_ChildList();
	unordered_set< pair<int, int>, intPairhash> getAnc_DescList();
	unordered_set<int> getRootList();

private:
	static unordered_map<int, pRelationships> parentTable;
	static unordered_set< pair<int, int>, intPairhash> parent_ChildPairList;
	static unordered_set< pair<int, int>, intPairhash> anc_DescPairList;
	static unordered_set<int> parentList;
	static unordered_set<int> childrenList;
	static unordered_set<int> rootList;
};