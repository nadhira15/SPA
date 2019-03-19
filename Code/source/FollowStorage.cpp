#include "FollowStorage.h"

unordered_map<int, fRelationships> FollowStorage::followTable;
unordered_set< pair<int, int>, intPairhash> FollowStorage::followPairList;
unordered_set< pair<int, int>, intPairhash> FollowStorage::followStarPairList;
unordered_set<int> FollowStorage::followerList;
unordered_set<int> FollowStorage::followedList;

FollowStorage::FollowStorage()
{
}

/*
	Adds the follows relation into the various lists in the storage
	Returns false if	1) the pair is already stored
						2) the followed statement has another follower stored
						3) the follower is following another statement
*/
bool FollowStorage::addFollow(int followed, int follower)
{
	// if follows Pair is already added
	if (!followPairList.emplace(pair<int, int>(followed, follower)).second)
	{
		return false;
	}

	if (followTable.find(followed) != followTable.end() &&
		followTable.find(followed)->second.follower != 0)
	{
		followPairList.erase(pair<int, int>(followed, follower));
		return false;
	}

	if (followTable.find(follower) != followTable.end() &&
		followTable.find(follower)->second.followed != 0)
	{
		followPairList.erase(pair<int, int>(followed, follower));
		return false;
	}

	// if followed is a new statement
	if (!followTable.emplace(followed, fRelationships{ 0, follower, {}, {} }).second)
	{
		followTable.find(followed)->second.follower = follower;
	}

	// if follower is a not new statement
	if (!followTable.emplace(follower, fRelationships{ followed, 0, {}, {} }).second)
	{
		followTable.find(follower)->second.followed = followed;
	}

	followedList.emplace(followed);
	followerList.emplace(follower);
	return true;
}

/*
	Sets "allFollowers" of followed
	Each followed - follower pair is entered into followStarPairList
	If followed already has a list of followers, it is not replaced and it return false
*/
bool FollowStorage::setAllFollowing(int followed, unordered_set<int> followers)
{
	if (followTable.find(followed)->second.allFollowers.size() != 0)
	{
		return false;
	}

	followTable.find(followed)->second.allFollowers = followers;

	for (auto itr = followers.cbegin(); itr != followers.cend(); ++itr)
	{
		followStarPairList.emplace(pair<int, int>(followed, *itr));
	}
	return true;
}

/*
	Sets "allFollowed" of follower
	Each followed - follower pair is entered into followStarPairList
	If follower already has a list of followed, it is not replaced and it return false
*/
bool FollowStorage::setAllFollowedBy(int follower, unordered_set<int> followed)
{
	if (followTable.find(follower)->second.allFollowed.size() != 0)
	{
		return false;
	}

	followTable.find(follower)->second.allFollowed = followed;

	for (auto itr = followed.cbegin(); itr != followed.cend(); ++itr)
	{
		followStarPairList.emplace(pair<int, int>(*itr, follower));
	}
	return true;
}

bool FollowStorage::isEmpty()
{
	return followTable.size() == 0;
}

// returns true if follows* pair is found
bool FollowStorage::hasFollowStarPair(pair<int, int> pair)
{
	return followStarPairList.find(pair) != followStarPairList.end();
}

/*
	return the statement following the statement specified
	return 0 if 'stm' is not found
*/
int FollowStorage::getFollower(int stm)
{
	if (followTable.find(stm) != followTable.end())
	{
		return followTable.at(stm).follower;
	}
	return 0;
}

/*
	return the statement followed by the statement specified
	return 0 if 'stm' is not found
*/
int FollowStorage::getStmFollowedBy(int stm)
{
	if (followTable.find(stm) != followTable.end())
	{
		return followTable.at(stm).followed;
	}
	return 0;
}

/*
	return a list of statements that is directly/indirectly following the statement specified
	return an empty set if 'stm' is not found
*/
unordered_set<int> FollowStorage::getAllFollowing(int stm)
{
	if (followTable.find(stm) != followTable.end())
	{
		return followTable.at(stm).allFollowers;
	}
	return {};
}

/*
	return a list of statements that is directly/indirectly followed by the statement specified
	return an empty set if 'stm' is not found
*/
unordered_set<int> FollowStorage::getAllFollowedBy(int stm)
{
	if (followTable.find(stm) != followTable.end())
	{
		return followTable.at(stm).allFollowed;
	}
	return {};
}

unordered_set<int> FollowStorage::getAllFollowers()
{
	return followerList;
}

unordered_set<int> FollowStorage::getAllFollowed()
{
	return followedList;
}

unordered_set<pair<int, int>, intPairhash> FollowStorage::getFollowPairs()
{
	return followPairList;
}

unordered_set<pair<int, int>, intPairhash> FollowStorage::getFollowStarPairs()
{
	return followStarPairList;
}
