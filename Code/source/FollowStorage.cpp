#include "FollowStorage.h"

FollowStorage::FollowStorage()
{
}

/*
	Pre-cond: Follows relations must be added in sequential order.
			  Meaning adding follows(2,3) followed by follows(1,2) is not allowed
	Adds the follows relation into the various lists in the storage
*/
bool FollowStorage::addFollowPair(int followed, int follower)
{
	pair<unordered_map<int, fRelationships>::iterator, bool> itrPair1;
	pair<unordered_map<int, fRelationships>::iterator, bool> itrPair2;

	//attempt to insert a new statement and its follower
	itrPair1 = followTable.emplace(followed, fRelationships{ 0, follower, {}, {} });
	itrPair2 = followTable.emplace(follower, fRelationships{ followed, 0, {}, {} });

	// As follows relation must be added in sequential order, the follower must be a new statement
	if (!itrPair2.second)
	{
		if (itrPair1.second)	// If a new followed has been successfully added, erase it
		{
			followTable.erase(followed);
		}
		return false;
	}
	else if (!itrPair1.second)
	{
		itrPair1.first->second.next = follower;
	}

	followPairList.emplace(pair<int, int>(followed, follower));
	followedList.emplace(followed);
	followerList.emplace(follower);
	return true;
}

/*
	adds the followers* relation into the various lists in the storage
*/
bool FollowStorage::addFollow_S_Pair(int followed, int follower)
{
	pair<unordered_set<int>::iterator, bool> itrPair1, itrPair2;

	//attempt to insert a new follower and followed
	itrPair1 = followTable.at(followed).allNext.emplace(follower);
	itrPair2 = followTable.at(follower).allPrevious.emplace(followed);

	//if either follower or followed was already added, erase the other if it was added successfully
	if (!itrPair1.second)
	{
		if (itrPair2.second)
		{
			followTable.find(follower)->second.allPrevious.erase(followed);
		}
		return false;
	}
	if (!itrPair2.second)
	{
		if (itrPair1.second)
		{
			followTable.find(followed)->second.allNext.erase(follower);
		}
		return false;
	}

	follow_S_PairList.emplace(pair<int, int>(followed, follower));
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
	return followTable.size == 0;
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
