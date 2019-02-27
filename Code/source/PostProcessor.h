#pragma once

#include <unordered_set>
#include <vector>

using namespace std;

#include "PKB.h"

class PostProcessor {
public:
	static void process(PKB pkb);

private:
	static PKB pkb;
	static void processFollows();
	static void processParent();
	static void traverseFamilyTree(int curr, unordered_set<int> ancestry,
						 unordered_map<int, unordered_set<int> > ancTable,
						 unordered_map<int, unordered_set<int> > descTable);
	static void processUses();
	static void processModifies();
};