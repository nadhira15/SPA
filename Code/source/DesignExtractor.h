#pragma once

using namespace std;

#include "PKB.h"

class DesignExtractor {
public:
	static void extractDesigns(PKB pkb);

private:
	static PKB pkb;
	static void processFollowStar();
	static void processParentStar();
	static void processUsesProcedures();
	static void processUsesContainers();
	static void processModifiesProcedures();
	static void processModifiesContainers();
};