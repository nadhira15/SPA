#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

#include "Hasher.h"

/*
	A structure to contain:	the program line to be executed next (next),
							the program line previously executed (previous),
	If such program line does not exist, it'll be set to 0
*/
struct nRelationships
{
	unordered_set<int> next;
	unordered_set<int> previous;
};

/*
	A storage class to store the Next relationships of program lines
*/
class NextStorage {
public:
	NextStorage();

	/*
		Adds the next relation into the various lists in the storage
		Returns false if	1) the pair is already stored
	*/
	bool addNext(int prev, int next);

	// returns true if nextTable is empty
	bool isEmpty();

	/*
		return a list of program lines that can be executed after 'line'
		return {} if 'line' is not found
	*/
	unordered_set<int> getNext(int line);

	/*
		return a list of program lines that can be executed before 'line'
		return {} if 'line' is not found
	*/
	unordered_set<int> getPrev(int line);

	// returns a list of all program lines that is executed after another
	unordered_set<int> getAllNext();

	// returns a list of all program lines that is before after another
	unordered_set<int> getAllPrev();

	// returns a list of all next pairs
	unordered_set< pair<int, int>, intPairhash> getNextPairs();

private:
	static unordered_map<int, nRelationships> nextTable;
	static unordered_set< pair<int, int>, intPairhash> nextPairList;
	static unordered_set<int> nextList;
	static unordered_set<int> prevList;
};
