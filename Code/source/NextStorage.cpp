#include "NextStorage.h"


unordered_map<int, nRelationships> NextStorage::nextTable;
unordered_set< pair<int, int>, intPairhash> NextStorage::nextPairList;
unordered_set<int> NextStorage::nextList;
unordered_set<int> NextStorage::prevList;

NextStorage::NextStorage()
{
}

bool NextStorage::addNext(int prev, int next)
{
	// if next Pair is already added
	if (!nextPairList.emplace(pair<int, int>(prev, next)).second)
	{
		return false;
	}

	// if prev already exist in nextTable
	if (!nextTable.emplace(prev, nRelationships{ { next }, {} }).second)
	{
		nextTable.find(prev)->second.next.emplace(next);
	}

	// if next already exist in nextTable
	if (!nextTable.emplace(next, nRelationships{ {}, { prev } }).second)
	{
		nextTable.find(next)->second.previous.emplace(prev);
	}

	nextList.emplace(next);
	prevList.emplace(prev);
	return true;
}

bool NextStorage::isEmpty()
{
	return nextTable.size() == 0;
}

unordered_set<int> NextStorage::getNext(int ln)
{
	if (nextTable.find(ln) != nextTable.end())
	{
		return nextTable.at(ln).next;
	}
	return {};
}

unordered_set<int> NextStorage::getPrev(int ln)
{
	if (nextTable.find(ln) != nextTable.end())
	{
		return nextTable.at(ln).previous;
	}
	return {};
}

unordered_set<int> NextStorage::getAllNext()
{
	return nextList;
}

unordered_set<int> NextStorage::getAllPrev()
{
	return prevList;
}

unordered_set<pair<int, int>, intPairhash> NextStorage::getNextPairs()
{
	return nextPairList;
}
