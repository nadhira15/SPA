#include "ParentStorage.h"

ParentStorage::ParentStorage()
{
}

/*
	Pre-cond: Parent-child relations must be added in sequential order.
			  Meaning adding Parent(2,3) followed by Parent(1,2) is not allowed
	Adds the parent relation into the various lists in the storage
*/
bool ParentStorage::addParent_Child(int parent, int child)
{
	pair<unordered_map<int, pRelationships>::iterator, bool> itrPair1;
	pair<unordered_map<int, pRelationships>::iterator, bool> itrPair2;

	//attempt to insert a new parent statement and its child
	itrPair1 = parentTable.emplace(parent, pRelationships{ 0, {child}, {}, {} });
	itrPair2 = parentTable.emplace(child, pRelationships{ parent, {}, {}, {} });

	// As parent relation must be added in sequential order, the child must be a new statement
	if (!itrPair2.second)
	{
		if (itrPair1.second)	// If a new parent has been successfully added, erase it
		{
			parentTable.erase(parent);
		}
		return false;
	}
	else if (!itrPair1.second)
	{
		itrPair1.first->second.children.emplace(child);
	}

	parent_ChildPairList.emplace(pair<int, int>(parent, child));
	parentList.emplace(parent);
	childrenList.emplace(child);
	return true;
}

bool ParentStorage::addAnc_Desc(int ancestor, int descendant)
{
	pair<unordered_set<int>::iterator, bool> itrPair1, itrPair2;

	//attempt to insert a new ancestor and descendant
	itrPair1 = parentTable.at(ancestor).descendants.emplace(descendant);
	itrPair2 = parentTable.at(descendant).ancestors.emplace(ancestor);

	//if either ancestor or descendant was already added, erase the other if it was added successfully
	if (!itrPair1.second)
	{
		if (itrPair2.second)
		{
			parentTable.find(descendant)->second.ancestors.erase(ancestor);
		}
		return false;
	}
	if (!itrPair2.second)
	{
		if (itrPair1.second)
		{
			parentTable.find(ancestor)->second.descendants.erase(descendant);
		}
		return false;
	}

	anc_DescPairList.emplace(pair<int, int>(ancestor, descendant));
	return true;
}

bool ParentStorage::setAncestors(int descendant, unordered_set<int> ancestors)
{
	if (parentTable.find(descendant)->second.ancestors.size() != 0)
	{
		return false;
	}
	parentTable.find(descendant)->second.ancestors = ancestors;
	return true;
}

bool ParentStorage::setDescendants(int ancestor, unordered_set<int> descendants)
{
	if (parentTable.find(ancestor)->second.descendants.size() != 0)
	{
		return false;
	}
	parentTable.find(ancestor)->second.descendants = descendants;
	return true;
}

bool ParentStorage::isEmpty()
{
	return parentTable.size() == 0;
}

bool ParentStorage::isParent(int stm)
{
	return parentList.find(stm) != parentList.end();
}

bool ParentStorage::isChild(int stm)
{
	return childrenList.find(stm) != childrenList.end();
}

bool ParentStorage::containsAnc_Desc(pair<int, int> pair)
{
	return anc_DescPairList.find(pair) != anc_DescPairList.end();
}

int ParentStorage::getParentOf(int stm)
{
	return parentTable.find(stm)->second.parent;
}

unordered_set<int> ParentStorage::getChildrenOf(int stm)
{
	return parentTable.find(stm)->second.children;
}

unordered_set<int> ParentStorage::getAncestorsOf(int stm)
{
	return parentTable.find(stm)->second.ancestors;
}

unordered_set<int> ParentStorage::getDescendantsOf(int stm)
{
	return parentTable.find(stm)->second.descendants;
}

unordered_set<int> ParentStorage::getParentList()
{
	return parentList;
}

unordered_set<int> ParentStorage::getChildrenList()
{
	return childrenList;
}

unordered_set<pair<int, int>> ParentStorage::getParent_ChildList()
{
	return parent_ChildPairList;
}

unordered_set<pair<int, int>> ParentStorage::getAnc_DescList()
{
	return anc_DescPairList;
}
