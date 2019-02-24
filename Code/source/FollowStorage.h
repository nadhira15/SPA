#pragma once

#include <exception>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

struct fRelationships
{
	int previous;
	int next;
	unordered_set<int> allPrevious;
	unordered_set<int> allNext;
};

class FollowStorage {
public:
	FollowStorage();

	bool addFollowPair(int followed, int follower);
	bool addFollow_S_Pair(int followed, int follower);

	bool isEmpty();
	bool containsFSPair(pair<int, int> pair);

	int getNextOf(int index);
	int getPrevOf(int index);
	unordered_set<int> getAllFollowing(int index);
	unordered_set<int> getAllFollowedBy(int index);
	unordered_set<int> getFollowerList();
	unordered_set<int> getFollowedList();
	unordered_set< pair<int, int> > getFPairList();
	unordered_set< pair<int, int> > getF_S_PairList();

private:
	static unordered_map<int, fRelationships> followTable;
	static unordered_set< pair<int, int> > followPairList;
	static unordered_set< pair<int, int> > follow_S_PairList;
	static unordered_set<int> followerList;
	static unordered_set<int> followedList;
};