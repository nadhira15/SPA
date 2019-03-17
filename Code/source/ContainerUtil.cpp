#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <algorithm>

using namespace std;

#include "PKB.h"
#include "QueryEvaluator.h"	
#include "LexicalToken.h"
#include "ExpressionUtil.h"
#include "ContainerUtil.h"

/*
The function transforms a vector of integers
into a set of strings
*/
unordered_set<string> ContainerUtil::to_strset(vector<int> intVec) {
	unordered_set<string> strSet;
	for (vector<int>::size_type i = 0; i != intVec.size(); i++) {
		strSet.insert(to_string(intVec[i]));
	}

	return strSet;
}

/*
The function transforms a vector of pair of integer and string
into a set of strings
*/
unordered_set<string> ContainerUtil::to_strset(vector<pair<int, string>> intStrVec) {
	unordered_set<string> strSet;
	for (vector<pair<int, string>>::size_type i = 0; i != intStrVec.size(); i++) {
		strSet.insert(to_string(intStrVec[i].first) + " " + intStrVec[i].second);
	}

	return strSet;
}

/*
The function transforms a set of integers
into a set of strings
*/
unordered_set<string> ContainerUtil::to_strset(unordered_set<int> intSet) {
	unordered_set<string> strSet;
	for (unordered_set<int>::iterator it = intSet.begin(); it != intSet.end(); ++it) {
		strSet.insert(to_string(*it));
	}

	return strSet;
}

/*
It transforms a set of pairs of integers into
a set of pairs of strings.
*/
unordered_set<string> ContainerUtil::to_strset(unordered_set<pair<int, int>, intPairhash> intPairSet) {
	unordered_set<string> strSet;
	for (unordered_set<pair<int, int>, intPairhash>::iterator it = intPairSet.begin(); it != intPairSet.end(); ++it) {
		pair<int, int> pointer = *it;
		strSet.insert(to_string(pointer.first) + " " + to_string(pointer.second));
	}

	return strSet;
}

/*
It transforms a set of pair of integer
and string into a set of strings consist
of an integer and a string
*/
unordered_set<string> ContainerUtil::to_strset(unordered_set<pair<int, string>, intStringhash> intStringSet) {
	unordered_set<string> strSet;
	for (unordered_set<pair<int, string>, intStringhash>::iterator it = intStringSet.begin(); it != intStringSet.end(); ++it) {
		pair<int, string> pointer = *it;
		strSet.insert(to_string(pointer.first) + " " + pointer.second);
	}

	return strSet;
}

/*
It transforms a set of pair of strings
into a set of strings of pairs.
*/
unordered_set<string> ContainerUtil::to_strset(unordered_set<pair<string, string>, strPairhash> strPairSet) {
	unordered_set<string> strSet;
	for (unordered_set<pair<string, string>, strPairhash>::iterator it = strPairSet.begin(); it != strPairSet.end(); ++it) {
		pair<string, string> pointer = *it;
		strSet.insert(pointer.first + " " + pointer.second);
	}

	return strSet;
}

/*
It fetchs out all the first member of
all pairs in the set
*/
unordered_set<string> ContainerUtil::getFirstParam(unordered_set<string> stringPair) {
	unordered_set<string> result;
	int spaceIndex;
	for (unordered_set<string>::iterator it = stringPair.begin(); it != stringPair.end(); ++it) {
		string pointer = *it;
		spaceIndex = pointer.find(" ");
		result.insert(pointer.substr(0, spaceIndex));
	}

	return result;
}

/*
It fetchs out all the second member
of all pairs in the set.
*/
unordered_set<string> ContainerUtil::getSecondParam(unordered_set<string> stringPair) {
	unordered_set<string> result;
	int spaceIndex;
	for (unordered_set<string>::iterator it = stringPair.begin(); it != stringPair.end(); ++it) {
		string pointer = *it;
		spaceIndex = pointer.find(" ");
		result.insert(pointer.substr(spaceIndex + 1, pointer.size() - spaceIndex - 1));
	}

	return result;
}

/*
Finds the intersection of 2 sets of string
*/
unordered_set<string> ContainerUtil::intersection(unordered_set<string> first, unordered_set<string> toContain) {
	unordered_set<string> result;
	for (unordered_set<string>::iterator it = first.begin(); it != first.end(); ++it) {
		string pointer = *it;
		if (toContain.count(pointer) == 1) {
			result.insert(pointer);
		}
	}

	return result;
}

/*
The function does the following:
1. It checks which pair in stringPair which
member in position "position"(either 1 or 2)
is in the toContain set.
2. It fetch out all the member in the
other "position" of the filtered set of pairs.
*/
unordered_set<string> ContainerUtil::getOtherPair(int position, unordered_set<string> stringPair, unordered_set<string> toContain) {
	unordered_set<string> result;
	int spaceIndex;
	string first;
	if (position == 1) {
		for (unordered_set<string>::iterator it = stringPair.begin(); it != stringPair.end(); ++it) {
			string pointer = *it;
			spaceIndex = pointer.find(" ");
			first = pointer.substr(0, spaceIndex);
			if (toContain.count(first) == 1) {
				result.insert(pointer.substr(spaceIndex + 1, pointer.size() - spaceIndex - 1));
			}
		}
	}
	if (position == 2) {
		for (unordered_set<string>::iterator it = stringPair.begin(); it != stringPair.end(); ++it) {
			string pointer = *it;
			spaceIndex = pointer.find(" ");
			first = pointer.substr(spaceIndex + 1, pointer.size() - spaceIndex - 1);
			if (toContain.count(first) == 1) {
				result.insert(pointer.substr(0, spaceIndex));
			}
		}
	}

	return result;
}

