#include "FollowStorage.h"

FollowStorage::FollowStorage()
{
}

/*
	Adds the follows relation into the various lists in the storage
	Returns false if	1) the pair is already stored
						2) the followed statement has another follower stored
						3) the follower is following another statement
*/
bool FollowStorage::addFollowPair(int followed, int follower)
{
	// if follows Pair is already added, return false
	if (!followPairList.emplace(pair<int, int>(followed, follower)).second)
	{
		return false;
	}

	// if followed exist in followTable and has "next" initialized, return false
	auto itrpr1 = followTable.emplace(followed, fRelationships{ 0, follower, {}, {} });
	if (!itrpr1.second && followTable.find(followed)->second.next != 0)
	{
		return false;
	}

	// if follower exist in followTable and has "previous" initialized, return false
	auto itrpr2 = followTable.emplace(follower, fRelationships{ followed, 0, {}, {} });
	if (!itrpr2.second && followTable.find(follower)->second.previous != 0)
	{
		return false;
	}

	// if a new followed statement, add it to the list of roots
	if (itrpr1.second)
	{
		rootList.emplace(followed);
	}
	else	// followed is not a statement and does not have "next" initialized
	{
		itrpr1.first->second.next = follower;
	}
	if (!itrpr2.second)		//follower is not a new statement and does not "previous" initialized
	{
		itrpr2.first->second.previous = followed;
		if (rootList.find(follower) != rootList.end())	//if follower was a root
		{
			rootList.erase(follower);
			int root = followed;
			while (followTable.find(root)->second.previous != 0)	//while root has a previous statement
			{
				root = followTable.find(root)->second.previous;
			}
			rootList.emplace(root);
		}
	}

	followedList.emplace(followed);
	followerList.emplace(follower);
	return true;
}

bool FollowStorage::setAllFollowing(int followed, unordered_set<int> followers)
{
	if (followTable.find(followed)->second.allNext.size() != 0)
	{
		return false;
	}
	followTable.find(followed)->second.allNext = followers;
	return true;
}

bool FollowStorage::setAllFollowedBy(int follower, unordered_set<int> followed)
{
	if (followTable.find(follower)->second.allPrevious.size() != 0)
	{
		return false;
	}
	followTable.find(follower)->second.allPrevious = followed;
	return true;
}

bool FollowStorage::isEmpty()
{
	return followTable.size() == 0;
}

// returns true if follows* pair is found
bool FollowStorage::containsFSPair(pair<int, int> pair)
{
	return follow_S_PairList.find(pair) != follow_S_PairList.end();
}

// return the statement following the statement specified by the index
int FollowStorage::getNextOf(int stm)
{
	return followTable.at(stm).next;
}

// return the statement being followed by the statement specified by the index
int FollowStorage::getPrevOf(int stm)
{
	return followTable.at(stm).previous;
}

// return a list of statements that is directly/indirectly following the statement specified by index
unordered_set<int> FollowStorage::getAllFollowing(int stm)
{
	return followTable.at(stm).allNext;
}

// return a list of statements that is directly/indirectly followed by the statement specified by index
unordered_set<int> FollowStorage::getAllFollowedBy(int stm)
{
	return followTable.at(stm).allPrevious;
}

unordered_set<int> FollowStorage::getFollowerList()
{
	return followerList;
}

unordered_set<int> FollowStorage::getFollowedList()
{
	return followedList;
}

unordered_set<pair<int, int>> FollowStorage::getFPairList()
{
	return followPairList;
}

unordered_set<pair<int, int>> FollowStorage::getF_S_PairList()
{
	return follow_S_PairList;
}

unordered_set<int> FollowStorage::getRoots()
{
	return rootList;
}
