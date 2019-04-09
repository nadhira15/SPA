#pragma once

#include "ParentStorage.h"

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
Parent
store.addParent_Child(5, 6);
store.addParent_Child(6, 7);
store.addParent_Child(5, 8);
store.addParent_Child(5, 9);
store.addParent_Child(11, 12);
store.setAncestors(6, {5});
store.setAncestors(7, {5, 6});
store.setAncestors(8, {5});
store.setAncestors(9, {5});
store.setAncestors(12, {11});
store.setDescendants(5, {6, 7, 8, 9});
store.setDescendants(6, {7});
store.setDescendants(11, {12});
*/

std::unordered_map<int, pRelationships> parentTable = { {5, {0, {6, 8, 9}, {}, {6, 7, 8, 9}}},
{6, {5, {7}, {5}, {7}}}, {7, {6, {}, {5, 6}, {}}}, {8, {5, {}, {5}, {}}}, {9, {5, {}, {5}, {}}},
{11, {0, {12}, {}, {12}}}, {12, {11, {}, {11}, {}}} };

std::unordered_set<int> parents = { 5, 6, 11 };
std::unordered_set<int> children = { 6, 7, 8, 9, 12 };

std::unordered_set<std::pair<int, int>, intPairhash> parentPair =
	{std::pair<int, int>(5, 6), std::pair<int, int>(6, 7), std::pair<int, int>(5, 8),
	std::pair<int, int>(5, 9), std::pair<int, int>(11, 12) };

std::unordered_set<std::pair<int, int>, intPairhash> parentStarPair =
	{std::pair<int, int>(5, 6), std::pair<int, int>(5, 7), std::pair<int, int>(5, 8),
	std::pair<int, int>(5, 9), std::pair<int, int>(6, 7), std::pair<int, int>(11, 12) };