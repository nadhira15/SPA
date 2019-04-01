#pragma once

#include "UseStorage.h"

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
Uses
ln9 - 12
store.addUsesStm(9, "apple");
store.addUsesStm(9, "melon");
store.addUsesStm(10, "banana");
store.addUsesStm(12, "melon");
store.addUsesStm(11, "melon");
store.addUsesStm(10, "melon");
store.addUsesProc("fruit2", "apple");
store.addUsesProc("fruit2", "melon");
store.addUsesProc("fruit2", "banana");
store.addUsesProc("fruit3", "melon");
store.addUsesProc("fruit2", "melon");
*/
unordered_set<string> stmVarList[4] = { {"apple", "melon"}, {"banana", "melon"}, {"melon"}, {"melon"} };
unordered_set<string> procVarList[2] = { {"apple", "melon", "banana"}, {"melon"} };
unordered_set<int> stmList[4] = { {9}, {9, 10, 11, 12}, {10}, {9, 10, 11, 12} };
unordered_set<string> procList[4] = { {"fruit2"}, {"fruit2", "fruit3"}, {"fruit2"}, {"fruit2", "fruit3"} };
unordered_set<pair<int, string>, intStringhash> stmVarPairs = { pair<int, string>(9, "apple"),
	pair<int, string>(9, "melon"), pair<int, string>(10, "banana"), pair<int, string>(10, "melon"),
	pair<int, string>(11, "melon"), pair<int, string>(12, "melon") };
unordered_set<pair<string, string>, strPairhash> procVarPairs = { 
	pair<string, string>("fruit2", "apple"), pair<string, string>("fruit2", "melon"),
	pair<string, string>("fruit2", "banana"), pair<string, string>("fruit3", "melon") };