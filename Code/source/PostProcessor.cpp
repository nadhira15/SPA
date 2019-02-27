#include "PostProcessor.h"

void PostProcessor::process(PKB storage)
{
	pkb = storage;
	processFollows();
	processParent();
	processUses();
	processModifies();
}

void PostProcessor::processFollows()
{
	unordered_set<int> history;
	unordered_map<int, unordered_set<int> > followerTable;
	unordered_map<int, unordered_set<int> > followedTable;
	unordered_set<int> rootList = pkb.getFollowRoots();

	// for all roots (statements that do follow another)
	for (auto itr1 = rootList.cbegin(); itr1 != rootList.cend(); ++itr1)
	{
		history = {};
		followerTable = {};
		followedTable = {};

		int curr = *itr1;
		while (curr != 0)
		{
			//add curr as an indirect follower for all previous statements
			for (auto itr2 = followerTable.begin(); itr2 != followerTable.end(); ++itr2)
			{
				itr2->second.emplace(curr);
			}
			followerTable.emplace(curr, unordered_set<int>{});
			followedTable.emplace(curr, history);
			history.emplace(curr);
			curr = pkb.getNxtStm(curr);
		}

		// transfer everything into PKB
		for (auto itr2 = followerTable.begin(); itr2 != followerTable.end(); ++itr2)
		{
			pkb.setFollowers(itr2->first, itr2->second);
			pkb.setStmFollowedBy(itr2->first, followedTable.find(itr2->first)->second);
		}
	}
}

void PostProcessor::processParent()
{
	unordered_set<int> ancestry;
	unordered_map<int, unordered_set<int> > descTable;
	unordered_map<int, unordered_set<int> > ancTable;
	unordered_set<int> rootList = pkb.getParentRoots();

	// for all roots (statements that do follow another)
	for (auto itr1 = rootList.cbegin(); itr1 != rootList.cend(); ++itr1)
	{
		ancestry = {};
		descTable = {};
		ancTable = {};

		traverseFamilyTree(*itr1, ancestry, ancTable, descTable);

		// transfer everything into PKB
		for (auto itr2 = descTable.begin(); itr2 != descTable.end(); ++itr2)
		{
			pkb.setDescendants(itr2->first, itr2->second);
			pkb.setAncestors(itr2->first, ancTable.find(itr2->first)->second);
		}
	}
}

void PostProcessor::traverseFamilyTree(int curr, unordered_set<int> ancestry,
							 unordered_map<int, unordered_set<int>> ancTable,
							 unordered_map<int, unordered_set<int>> descTable)
{
	for (auto ancestor = ancestry.begin(); ancestor != ancestry.end(); ++ancestor)
	{
		descTable.find(*ancestor)->second.emplace(curr);
	}
	descTable.emplace(curr, unordered_set<int>{});
	ancTable.emplace(curr, ancestry);
	ancestry.emplace(curr);

	for (auto child = pkb.getAllChildren().begin(); child != pkb.getAllChildren().end(); ++child)
	{
		traverseFamilyTree(*child, ancestry, ancTable, descTable);
	}
}

void PostProcessor::processUses()
{
	string procedure = pkb.getProcName();
	unordered_set< pair<int, string>, intStringhash> usePairs = pkb.getStmVarUsePairs();

	for (auto itr = usePairs.cbegin(); itr != usePairs.cend(); ++itr)
	{
		pkb.addUses(procedure, itr->second);
		unordered_set<int> ancList = pkb.getAllAncestors(itr->first);
		for (auto ancestor = ancList.cbegin(); ancestor != ancList.cend(); ++ancestor)
		{
			pkb.addUses(*ancestor, itr->second);
		}
	}
}

void PostProcessor::processModifies()
{
	string procedure = pkb.getProcName();
	unordered_set< pair<int, string>, intStringhash> modifyPairs = pkb.getStmVarModifyPairs();

	for (auto itr = modifyPairs.cbegin(); itr != modifyPairs.cend(); ++itr)
	{
		pkb.addModifies(procedure, itr->second);
		unordered_set<int> ancList = pkb.getAllAncestors(itr->first);
		for (auto ancestor = ancList.cbegin(); ancestor != ancList.cend(); ++ancestor)
		{
			pkb.addModifies(*ancestor, itr->second);
		}
	}
}
