#include "DesignExtractor.h"

PKB DesignExtractor::pkb;

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

void DesignExtractor::processFollowStar()
{
	int stmtNum = pkb.getTotalStmNo();

	for (int i = stmtNum - 1; i >= 0; i--) {
		int currStmt = i;
		int directFollowStm = pkb.getNxtStm(currStmt);
		//Has a Next Stm
		if (directFollowStm != 0) {
			unordered_set<int> followedByStar = pkb.getAllFollowedBy(directFollowStm);
			followedByStar.insert(directFollowStm);
			pkb.setStmFollowedBy(currStmt, followedByStar);
		}
	}

	for (int i = 1; i <= stmtNum; i++) {
		int currStmt = i;
		int directPrvStm = pkb.getPrvStm(currStmt);

		//If have Prev Stm
		if (directPrvStm != 0) {
			unordered_set<int>  followStar = pkb.getAllFollowing(directPrvStm);
			followStar.insert(directPrvStm);
			pkb.setFollowers(currStmt, followStar);
		}
	}

}

void DesignExtractor::processParentStar()
{
	int stmtNum = pkb.getTotalStmNo();

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

	for (int i = stmtNum - 1; i >= 0; i--) {
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
			}
		}
	}
}

void DesignExtractor::processUsesProcedures()
{
}

void DesignExtractor::processModifiesProcedures()
{
}

void DesignExtractor::processModifiesContainers()
{
	int stmtNum = pkb.getTotalStmNo();

	for (int i = 1; i <= stmtNum; i++) {
		int currLine = i;
		unordered_set<int> descendents = pkb.getAllDescendants(currLine);
		for (int descendent : descendents) {
			//unordered_set<string> modifiedList = pkb.getModifiedVar(descendent);
			//for (string modifiedVar : modifiedList) {
			//	pkb.addModifies(currLine, modifiedVar);
			//}
		}
	}
}

void DesignExtractor::processUsesContainers()
{
	int stmtNum = pkb.getTotalStmNo();

	for (int i = 0; i <= stmtNum; i++) {
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
