#include "FollowStorage.h"

FollowStorage::FollowStorage()
{
}

/*
	adds the follows related statements into the various lists in the storage
*/
bool FollowStorage::addFollowPair(int followed, int follower)
{
	pair<unordered_map<int, fRelationships>::iterator, bool> itrPair;

	//attempt to insert a new element with next initialized
	itrPair = followTable.emplace(followed, fRelationships{ 0, follower, {}, {} });

	//if element already exist and next is not initialized
	if (!itrPair.second && itrPair.first->second.next == 0)
	{
		itrPair.first->second.next = follower;
	}
	else if (!itrPair.second)	//if next is already initialized
	{
		return false;
	}

	//attempt to insert a new element with previous initialized
	itrPair = followTable.emplace(follower, fRelationships{ followed, 0, {}, {} });

	//if element already exist and previous is not initialized
	if (!itrPair.second && itrPair.first->second.previous == 0)
	{
		itrPair.first->second.previous = followed;
	}
	else if (!itrPair.second)	//if previous is already initialized
	{
		return false;
	}

	followPairList.emplace(pair<int, int>(followed, follower));
	followedList.emplace(followed);
	followerList.emplace(follower);
	return true;
}

/*
	adds the followers* related statements into the various lists in the storage
*/
bool FollowStorage::addFollow_S_Pair(int followed, int follower)
{
	pair<unordered_set<int>::iterator, bool> itrPair1, itrPair2;

	//attempt to insert a new follower and followed
	itrPair1 = followTable.at(followed).allNext.emplace(follower);
	itrPair2 = followTable.at(follower).allPrevious.emplace(followed);

	//if either follower or followed was already added
	if (!itrPair1.second || !itrPair2.second)
	{
		return false;
	}

	follow_S_PairList.emplace(pair<int, int>(followed, follower));
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
int FollowStorage::getNextOf(int index)
{
	return followTable.at(index).next;
}

// return the statement being followed by the statement specified by the index
int FollowStorage::getPrevOf(int index)
{
	return followTable.at(index).previous;
}

// return a list of statements that is directly/indirectly following the statement specified by index
unordered_set<int> FollowStorage::getAllFollowing(int index)
{
	return followTable.at(index).allNext;
}

// return a list of statements that is directly/indirectly followed by the statement specified by index
unordered_set<int> FollowStorage::getAllFollowedBy(int index)
{
	return followTable.at(index).allPrevious;
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
