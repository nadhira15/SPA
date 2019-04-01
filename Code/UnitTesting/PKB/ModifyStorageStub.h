#pragma once

#include "ModifyStorage.h"

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
		}
	}

	procedure fruit2 {
9		pineapple = apple + melon;
10		while (10 > banana) {
11			call fruit3;
		}
	}

	procedure fruit3 {
12		print melon;
	}

*/

/*
Modify
store.addModifiesStm(1, "apple");
store.addModifiesStm(2, "pineapple");
store.addModifiesStm(4, "apple");
store.addModifiesStm(5, "durian");
store.addModifiesStm(5, "figs");
store.addModifiesStm(6, "durian");
store.addModifiesStm(7, "durian");
store.addModifiesStm(8, "figs");
store.addModifiesStm(9, "pineapple");
store.addModifiesProc("fruit", "apple");
store.addModifiesProc("fruit", "pineapple");
store.addModifiesProc("fruit", "durian");
store.addModifiesProc("fruit", "figs");
store.addModifiesProc("fruit2", "pineapple");
*/
unordered_set<string> ModifyStmVarList[8] = { {"apple"}, {"pineapple"}, {"apple"}, {"durian", "figs"},
{"durian"}, {"durian"}, {"figs"}, {"pineapple"} };
unordered_set<string> ModifyProcVarList[2] = { {"apple", "pineapple", "durian", "figs"}, {"pineapple"} };
unordered_set<int> ModifyStmList[5] = { {1, 4}, {2, 9}, {5, 6, 7}, {5, 8}, {1, 2, 4, 5, 6, 7, 8, 9} };
unordered_set<string> ModifyProcList[5] = { {"fruit"}, {"fruit", "fruit2"}, {"fruit"}, {"fruit"},
{"fruit", "fruit2"} };
unordered_set<pair<int, string>, intStringhash> ModifyStmVarPairs = { pair<int, string>(1, "apple"),
	pair<int, string>(2, "pineapple"), pair<int, string>(4, "apple"), pair<int, string>(5, "durian"),
	pair<int, string>(5, "figs"), pair<int, string>(6, "durian"), pair<int, string>(7, "durian"),
	pair<int, string>(8, "figs"), pair<int, string>(9, "pineapple") };
unordered_set<pair<string, string>, strPairhash> ModifyProcVarPairs = {
	pair<string, string>("fruit", "apple"), pair<string, string>("fruit", "pineapple"),
	pair<string, string>("fruit", "durian"), pair<string, string>("fruit", "figs"),
	pair<string, string>("fruit2", "pineapple")};