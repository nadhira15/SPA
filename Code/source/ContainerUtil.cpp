#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <algorithm>

#include "PKB.h"
#include "QueryEvaluator.h"	
#include "LexicalToken.h"
#include "ExpressionUtil.h"
#include "ContainerUtil.h"

/*
The function returns a map of string to
vector of strings given a key and vector 
of integers.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key, std::vector<int> intVec) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> keyValue;
	for (vector<int>::size_type i = 0; i != intVec.size(); i++) {
		keyValue.push_back(to_string(intVec[i]));
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair (key, keyValue);
	mapvec.insert(keyValuePair);

	return mapvec;
}

/*
The function returns a map of strings
to vectors of strings given two keys and
vector of pairs of int and string.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key1, 
	std::string key2, std::vector<pair<int, std::string>> intStrVec) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> key1Value;
	std::vector<std::string> key2Value;
	for (std::vector<std::pair<int, std::string>>::size_type i = 0; i != intStrVec.size(); i++) {
		key1Value.push_back(to_string(intStrVec[i].first));
		key2Value.push_back(intStrVec[i].second);
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair1 (key1, key1Value);
	std::pair<std::string, std::vector<std::string>> keyValuePair2 (key2, key2Value);
	mapvec.insert(keyValuePair1);
	mapvec.insert(keyValuePair2);

	return mapvec;
}

/*
The function returns a map of string
to vector of string given a key and 
set of integers
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key,
	std::unordered_set<int> intSet) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> keyValue;
	for (std::unordered_set<int>::iterator it = intSet.begin(); it != intSet.end(); ++it) {
		keyValue.push_back(to_string(*it));
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair(key, keyValue);
	mapvec.insert(keyValuePair);

	return mapvec;
}

/*
The function returns a map of string
to vector of string given a key and
set of strings
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key,
	std::unordered_set<std::string> strSet) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> keyValue;
	for (std::unordered_set<std::string>::iterator it = strSet.begin(); it != strSet.end(); ++it) {
		keyValue.push_back(*it);
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair(key, keyValue);
	mapvec.insert(keyValuePair);

	return mapvec;
}

/*
The function returns a map of strings
to vectors of strings given two keys and
set of pairs of integers and integers.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key1,
	std::string key2, std::unordered_set<std::pair<int, int>, intPairhash> intPairSet) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> key1Value;
	std::vector<std::string> key2Value;
	for (std::unordered_set<std::pair<int, int>, intPairhash>::iterator it = intPairSet.begin(); it != intPairSet.end(); ++it) {
		std::pair<int, int> pointer = *it;
		key1Value.push_back(to_string(pointer.first));
		key2Value.push_back(to_string(pointer.second));
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair1(key1, key1Value);
	std::pair<std::string, std::vector<std::string>> keyValuePair2(key2, key2Value);
	mapvec.insert(keyValuePair1);
	mapvec.insert(keyValuePair2);

	return mapvec;
}

/*
The function returns a map of strings
to vectors of strings given two keys and
set of pairs of integers and strings.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key1,
	std::string key2, std::unordered_set<pair<int, std::string>, intStringhash> intStringSet) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> key1Value;
	std::vector<std::string> key2Value;
	for (std::unordered_set<std::pair<int, std::string>, intPairhash>::iterator it = intStringSet.begin(); it != intStringSet.end(); ++it) {
		std::pair<int, std::string> pointer = *it;
		key1Value.push_back(to_string(pointer.first));
		key2Value.push_back(pointer.second);
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair1(key1, key1Value);
	std::pair<std::string, std::vector<std::string>> keyValuePair2(key2, key2Value);
	mapvec.insert(keyValuePair1);
	mapvec.insert(keyValuePair2);

	return mapvec;
}

/*
The function returns a map of strings
to vectors of strings given two keys and
set of pairs of strings and strings.
*/
std::unordered_map<std::string, std::vector<std::string>> ContainerUtil::to_mapvec(std::string key1,
	std::string key2, std::unordered_set<pair<std::string, std::string>, strPairhash> strPairSet) {
	std::unordered_map<std::string, std::vector<std::string>> mapvec;
	std::vector<std::string> key1Value;
	std::vector<std::string> key2Value;
	for (std::unordered_set<std::pair<std::string, std::string>, strPairhash>::iterator it = strPairSet.begin(); it != strPairSet.end(); ++it) {
		std::pair<std::string, std::string> pointer = *it;
		key1Value.push_back(pointer.first);
		key2Value.push_back(pointer.second);
	}
	std::pair<std::string, std::vector<std::string>> keyValuePair1(key1, key1Value);
	std::pair<std::string, std::vector<std::string>> keyValuePair2(key2, key2Value);
	mapvec.insert(keyValuePair1);
	mapvec.insert(keyValuePair2);

	return mapvec;
}

