#include "DesignExtractor.h"

PKB DesignExtractor::pkb;

/* The main public function that calls other function to populate different design entities in the PKB.
 */
void DesignExtractor::extractDesigns(PKB storage)
{
	pkb = storage;
	processFollowStar();
	processParentStar();
	processUsesProcedures();
	processUsesContainers();
	processModifiesProcedures();
	processModifiesContainers();
}

/*
 * Processes the Follow* Design Entitiy in the SPA requirement.
 */
void DesignExtractor::processFollowStar()
{
	int stmtNum = pkb.getTotalStmNo();

	//Process stmt list s where Follow*(index, s) is true
	for (int i = stmtNum; i >= 1; i--) {
		int currStmt = i;
		int directFollowStm = pkb.getNxtStm(currStmt);
		//Has a Next Stm
		if (directFollowStm != 0) {
			unordered_set<int> followedByStar = pkb.getAllFollowing(directFollowStm);
			followedByStar.insert(directFollowStm);
			pkb.setFollowers(currStmt, followedByStar);
		}
	}

	//Process stmt list s where Follow*(s, index) is true
	for (int i = 1; i <= stmtNum; i++) {
		int currStmt = i;
		int directPrvStm = pkb.getPrvStm(currStmt);

		//If have Prev Stm
		if (directPrvStm != 0) {
			unordered_set<int>  followStar = pkb.getAllFollowedBy(directPrvStm);
			followStar.insert(directPrvStm);
			pkb.setStmFollowedBy(currStmt, followStar);
		}
	}

}

/*
 * Processes the Parent* Design Entitiy in the SPA requirement.
 */
void DesignExtractor::processParentStar()
{
	int stmtNum = pkb.getTotalStmNo();

	//Process stmt list s where Parent*(s, index) is true.
	for (int i = 1 ; i <= stmtNum ; i++) {
		int currStmt = i;
		int directParentStm = pkb.getParent(i);

		//If have parent,
		if (directParentStm != 0) {
			unordered_set<int> ancestors = pkb.getAllAncestors(directParentStm);
			ancestors.insert(directParentStm);
			pkb.setAncestors(i, ancestors);
		}
	}

	//Process stmt list s where Parent*(index, s) is true.
	for (int i = stmtNum; i >= 1; i--) {
		int currStmt = i;
		unordered_set<int> directChildStm = pkb.getChildren(i);

		unordered_set<int> allDescendents;
		//Has a Child Stm
		if (!directChildStm.empty()) {
			for (int childStm : directChildStm) {
				unordered_set<int> descendents = pkb.getAllDescendants(childStm);
				for (int descendent : descendents) {
					allDescendents.insert(descendent);
				}
				allDescendents.insert(childStm);
			}
			pkb.setDescendants(i, allDescendents);
		}
	}
}

void DesignExtractor::processUsesProcedures()
{
	//TODO: Implement for Iteration 2
}

void DesignExtractor::processModifiesProcedures()
{
	//TODO: Implement for Iteration 2
}

/*
 * Processes the the variables that are MODIFIED in Containing Statement so that the While/If Statement
 * also MODIFY these variables.
 */
void DesignExtractor::processModifiesContainers()
{
	int stmtNum = pkb.getTotalStmNo();

	for (int i = stmtNum; i >= 1; i--) {
		int currLine = i;
		unordered_set<int> descendents = pkb.getAllDescendants(currLine);
		for (int descendent : descendents) {
			unordered_set<string> modifiedList = pkb.getModifiedVar(descendent);
			for (string modifiedVar : modifiedList) {
				pkb.addModifies(currLine, modifiedVar);
			}
		}
	}
}

/*
 * Processes the the variables that are USED in Containing Statement so that the While/If Statement
 * also USE these variables.
 */
void DesignExtractor::processUsesContainers()
{
	int stmtNum = pkb.getTotalStmNo();

	for (int i = stmtNum; i >= 1; i--) {
		int currLine = i;
		unordered_set<int> descendents = pkb.getAllDescendants(currLine);
		for (int descendent : descendents) {
			unordered_set<string> usedList = pkb.getUsedVar(descendent);
			for (string usedVariable : usedList) {
				pkb.addUses(currLine, usedVariable);
			}
		}
	}
}
