#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

#include "Hasher.h"

/* 
	A structure to contain:	the statement being followed directly (followed),
							it's immediate follower (follower),
							a list of statements that being directly/indirectly followed (allFollowed),
							a list of statements directly/indirectly following it (allFollowers)
	If such statement or list does not exist, it'll be set to 0 or {} respectively
*/
struct fRelationships
{
	int followed;
	int follower;
	unordered_set<int> allFollowed;
	unordered_set<int> allFollowers;
};

/*
	A storage class to store the Follows & Follows* relationships of statements
*/
class FollowStorage {
public:
	FollowStorage();

	bool addFollowPair(int followed, int follower);
	bool setAllFollowing(int followed, unordered_set<int> followers);
	bool setAllFollowedBy(int follower, unordered_set<int> followed);

	bool isEmpty();
	bool hasFollowStarPair(pair<int, int> pair);

	int getFollower(int stm);
	int getStmFollowedBy(int stm);
	unordered_set<int> getAllFollowing(int stm);
	unordered_set<int> getAllFollowedBy(int stm);
	unordered_set<int> getAllFollowers();
	unordered_set<int> getAllFollowed();
	unordered_set< pair<int, int>, intPairhash> getFollowPairs();
	unordered_set< pair<int, int>, intPairhash> getFollowStarPairs();

private:
	static unordered_map<int, fRelationships> followTable;
	static unordered_set< pair<int, int>, intPairhash> followPairList;
	static unordered_set< pair<int, int>, intPairhash> followStarPairList;
	static unordered_set<int> followerList;
	static unordered_set<int> followedList;
};