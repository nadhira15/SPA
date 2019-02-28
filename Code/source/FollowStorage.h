#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

#include "Hasher.h"

/* 
	A structure to contain:	the statement being followed directly (previous),
							it's immediate follower (next),
							a list of statements that being directly/indirectly followed (allPrevious),
							a list of statements directly/indirectly following it (allNext)
	If such statement or list does not exist, it'll be set to 0 or {} respectively
*/
struct fRelationships
{
	int previous;
	int next;
	unordered_set<int> allPrevious;
	unordered_set<int> allNext;
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
	bool containsFSPair(pair<int, int> pair);

	int getNextOf(int stm);
	int getPrevOf(int stm);
	unordered_set<int> getAllFollowing(int stm);
	unordered_set<int> getAllFollowedBy(int stm);
	unordered_set<int> getFollowerList();
	unordered_set<int> getFollowedList();
	unordered_set< pair<int, int>, intPairhash> getFPairList();
	unordered_set< pair<int, int>, intPairhash> getF_S_PairList();
	unordered_set<int> getRoots();

private:
	static unordered_map<int, fRelationships> followTable;
	static unordered_set< pair<int, int>, intPairhash> followPairList;
	static unordered_set< pair<int, int>, intPairhash> follow_S_PairList;
	static unordered_set<int> followerList;
	static unordered_set<int> followedList;
	static unordered_set<int> rootList;
};