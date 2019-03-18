#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"

class ContainerUtil
{
public:
	static unordered_set<string> to_strset(vector<int> intVec);
	static unordered_set<string> to_strset(vector<pair<int, string>> intStrVec);
	static unordered_set<string> to_strset(unordered_set<int> intSet);
	static unordered_set<string> to_strset(unordered_set<pair<int, int>, intPairhash> intPairSet);
	static unordered_set<string> to_strset(unordered_set<pair<int, string>, intStringhash> intStringSet);
	static unordered_set<string> to_strset(unordered_set<pair<string, string>, strPairhash> strPairSet);
	static unordered_set<string> getFirstParam(unordered_set<string> stringPair);
	static unordered_set<string> getSecondParam(unordered_set<string> stringPair);
	static unordered_set<string> intersection(unordered_set<string> first, unordered_set<string> toContain);
	static unordered_set<string> getOtherPair(int position, unordered_set<string> stringPair, unordered_set<string> toContain);
};
