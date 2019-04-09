#pragma once

#include "FollowStorage.h"

/*
	procedure fruit {
1		read apple;
2		call fruit2;
3		print banana;
4		apple = cherry + 10 * banana;
5		if (apple <= 0) then {
6			while ( durian > cherry ) {
7				durian = grapes + banana;
			}
		} else {
8			figs = apple * melon / cherry;
9			call fruit3;
		}
	}

	procedure fruit2 {
10		pineapple = apple + melon;
11		while (10 > banana) {
12			call fruit3;
		}
	}

	procedure fruit3 {
13		print melon;
14		call fruit4;
	}

	procedure fruit4 {
15		pear = jackfruit + apple;
	}
*/
/*
Follows
store.addFollow(1, 2);
store.addFollow(2, 3);
store.addFollow(3, 4);
store.addFollow(4, 5);
store.addFollow(8, 9);
store.addFollow(10, 11);
store.addFollow(13, 14);
store.setAllFollowing(1, {2, 3, 4, 5});
store.setAllFollowing(2, {3, 4, 5});
store.setAllFollowing(3, {4, 5});
store.setAllFollowing(4, {5});
store.setAllFollowing(8, {9});
store.setAllFollowing(10, {11});
store.setAllFollowing(13, {14});
store.setAllFollowedBy(2, {1});
store.setAllFollowedBy(3, {1, 2});
store.setAllFollowedBy(4, {1, 2, 3});
store.setAllFollowedBy(5, {1, 2, 3, 4});
store.setAllFollowedBy(9, {8});
store.setAllFollowedBy(11, {10});
store.setAllFollowedBy(14, {13});
*/

std::unordered_map<int, fRelationships> followTable = { {1, {0, 2, {}, {2, 3, 4, 5}}},
{2, {1, 3, {1}, {3, 4, 5}}}, {3, {2, 4, {1, 2}, {4, 5}}}, {4, {3, 5, {1, 2, 3}, {5}}},
{5, {4, 0, {1, 2, 3, 4}, {}}}, {8, {0, 9, {}, {9}}}, {9, {8, 0, {8}, {}}}, {10, {0, 11, {}, {11}}},
{11, {10, 0, {10}, {}}}, {13, {0, 14, {}, {14}}}, {14, {13, 0, {13}, {}}} };

std::unordered_set<int> followers = {2, 3, 4, 5, 9, 11, 14};
std::unordered_set<int> followed = {1, 2, 3, 4, 8, 10, 13};

std::unordered_set<std::pair<int, int>, intPairhash> followPairs = { std::pair<int, int>(1, 2),
	std::pair<int, int>(2, 3), std::pair<int, int>(3, 4), std::pair<int, int>(4, 5),
	std::pair<int, int>(8, 9), std::pair<int, int>(10, 11), std::pair<int, int>(13, 14) };

std::unordered_set<std::pair<int, int>, intPairhash> followStarPairs = { std::pair<int, int>(1, 2),
	std::pair<int, int>(1, 3), std::pair<int, int>(1, 4), std::pair<int, int>(1, 5),
	std::pair<int, int>(2, 3), std::pair<int, int>(2, 4), std::pair<int, int>(2, 5),
	std::pair<int, int>(3, 4), std::pair<int, int>(3, 5), std::pair<int, int>(4, 5),
	std::pair<int, int>(8, 9), std::pair<int, int>(10, 11), std::pair<int, int>(13, 14) };
