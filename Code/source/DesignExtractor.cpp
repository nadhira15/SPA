#include "DesignExtractor.h"

PKB DesignExtractor::pkb;

/* The main public function that calls other function to populate different design entities in the PKB.
 */
void DesignExtractor::extractDesigns(PKB storage)
{
	pkb = storage;

	//Verification
	verifyCalledProceduresPresence();
	verifyAbsenceOfCyclicity();
	
	//Statement-Statement Relationships
	processFollowStar();
	processParentStar();

	//Statement/Procedure - Variable Relationships
	vector<string> sortedProcedures = topologicalSortProcedures();
	processAdvancedUsesAndModifies(sortedProcedures);
}

/*
 *Checks if the procedures found in Call Statements are found inside the procedure list.
 * Iterates through the list of Call Statements and check if it exist in procedure list.
 */
void DesignExtractor::verifyCalledProceduresPresence()
{
/*	std::unordered_set<std::string> calledProcedures = pkb.getAllCallee();
	std::unoredred_set<std::string> procedureList = pkb.getProcedures();
	for (string calledProcedure : calledProcedures) {
		bool present = false;
		for (string procedure : procedureList) {
			if (calledProcedure == procedure) {
				present = true;
				break;
			}
		}
		if (present = false) {
			throw "A procedure that was called does not exist";
		}
	}
*/	
}

void DesignExtractor::verifyAbsenceOfCyclicity() {

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

vector<string> DesignExtractor::topologicalSortProcedures()
{
	/*
	unordered_set<std::string> procList = pkb.getProcedures();
	int procListSize = procList.size();

	unordered_set<std::string> visitedProcedures;
	vector<std::string> sortedProcedures(procListSize);

	for (std::string procedure : procList) {
		//Try finding if the procedure has already been visited.
		std::unordered_set<std::string>::const_iterator exist = visitedProcedures.find(procedure);

		//If we have not visited this procedure, visit it.
		if (exist != visitedProcedures.end()) {
			DFSRecursive(procedure, visitedProcedures, sortedProcedures);
		}
	}
	*/

	vector<std::string> sortedProcedures(5);
	return sortedProcedures;
}

void DesignExtractor::DFSRecursive(std::string procedure, unordered_set<std::string> &visitedProcedures, vector<std::string> &sortedProcedures) {
	/*
	//Marks procedure as visited
	visitedProcedures.insert(procedure);

	//Get neighbouring procedures.
	unordered_set<string> adjacentProcedures = pkb.getCallee(procedure);

	//For each neighbouring procedure
	for (std::string callee : adjacentProcedures) {

		//Check if neighbouring procedure has been visited
		std::unordered_set<std::string>::const_iterator exist = visitedProcedures.find(callee);

		//If neighbouring procedure has not been visited, visit it.
		if (exist != visitedProcedures.end()) {
			DFSRecursive(procedure, visitedProcedures);
		}
	}

	//Add procedure to the sortedProcedures.
	sortedProcedures.push_back(procedure);
	*/
}

void DesignExtractor::processAdvancedUsesAndModifies(std::vector<std::string> sortedProcedures) {
	for (std::string procedure : sortedProcedures) {
		/*
		processCallUses(procedure);
		processCallModifies(procedure);
		processUsesContainers(procedure);
		processModifiesContainers(procedure);
		processUsesProcedures(procedure);
		processModifiesProcedures(procedure);
		*/
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
