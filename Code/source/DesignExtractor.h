#pragma once
#include "PKB.h"

/* Design Extractor performs Design Entitiy Extraction for the following relations
 * 1) Follow*
 * 2) Parent*
 * 3) While/IfStatement Modify variables in Containing Statement
 * 4) While/IfStatemnt Using variables in Containing Statement
 * 5) Procedure Modify variables in statement list.
 * 6) Procedure Using variables in statement list.
 *
 * IMPORTANT NOTE: Call DesignExtractor only after populating PKB with the basic relations
 * 
 * To use DesignExtractor:
 * 1) call extractDesigns() with the intended target PKB as the argument.
 * 
 */
class DesignExtractor {
public:
	static void extractDesigns(PKB storage);

private:
	static PKB pkb;
	static void verifyCalledProceduresPresence();
	static void processFollowStar();
	static void processParentStar();
	static void topologicalSortProcedures();
	static void processCallUses();
	static void processCallModifies();
	static void processUsesProcedures();
	static void processUsesContainers();
	static void processModifiesProcedures();
	static void processModifiesContainers();
};