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
unordered_set<string> stmVarList[8] = { {"apple", "melon"}, {"banana", "melon"}, {"melon"}, {"melon"} };
unordered_set<string> procVarList[2] = { {"apple", "melon", "banana"}, {"melon"} };
unordered_set<int> stmList[4] = { {9}, {9, 10, 11, 12}, {10}, {9, 10, 11, 12} };
unordered_set<string> procList[4] = { {"fruit2"}, {"fruit2", "fruit3"}, {"fruit2"}, {"fruit2", "fruit3"} };
unordered_set<pair<int, string>, intStringhash> stmVarPairs = { pair<int, string>(9, "apple"),
	pair<int, string>(9, "melon"), pair<int, string>(10, "banana"), pair<int, string>(10, "melon"),
	pair<int, string>(11, "melon"), pair<int, string>(12, "melon") };
unordered_set<pair<string, string>, strPairhash> procVarPairs = {
	pair<string, string>("fruit2", "apple"), pair<string, string>("fruit2", "melon"),
	pair<string, string>("fruit2", "banana"), pair<string, string>("fruit3", "melon") };