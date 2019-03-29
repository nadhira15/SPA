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
	processCallsStar(sortedProcedures);
}

/*
 *Checks if the procedures found in Call Statements are found inside the procedure list.
 * Iterates through the list of Call Statements and check if it exist in procedure list.
 */
void DesignExtractor::verifyCalledProceduresPresence()
{
	std::unordered_set<std::string> calledProcedures = pkb.getAllCallees();
	std::unordered_set<std::string> procedureList = pkb.getProcList();
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
		int directFollowStm = pkb.getFollower(currStmt);
		//Has a Next Stm
		if (directFollowStm != 0) {
			unordered_set<int> followedByStar = pkb.getAllFollowing(directFollowStm);
			followedByStar.insert(directFollowStm);
			pkb.setAllFollowing(currStmt, followedByStar);
		}
	}

	//Process stmt list s where Follow*(s, index) is true
	for (int i = 1; i <= stmtNum; i++) {
		int currStmt = i;
		int directPrvStm = pkb.getStmFollowedBy(currStmt);

		//If have Prev Stm
		if (directPrvStm != 0) {
			unordered_set<int>  followStar = pkb.getAllFollowedBy(directPrvStm);
			followStar.insert(directPrvStm);
			pkb.setAllFollowedBy(currStmt, followStar);
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
			unordered_set<int> ancestors = pkb.getAncestors(directParentStm);
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
				unordered_set<int> descendents = pkb.getDescendants(childStm);
				for (int descendent : descendents) {
					allDescendents.insert(descendent);
				}
				allDescendents.insert(childStm);
			}
			pkb.setDescendants(i, allDescendents);
		}
	}
}

vector<string> DesignExtractor::topologicalSortProcedures() {
	unordered_set<std::string> procList = pkb.getProcList();
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
	return sortedProcedures;
}

void DesignExtractor::DFSRecursive(std::string procedure, unordered_set<std::string> &visitedProcedures, vector<std::string> &sortedProcedures) {
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
			DFSRecursive(procedure, visitedProcedures, sortedProcedures);
		}
	}
	//Add procedure to the sortedProcedures.
	sortedProcedures.push_back(procedure);
}

/* 
 * Performs population of Uses and Modifies for Calls, If/While Container Statements and Procecedures
 * starting from the leaf node procedures.
 */
void DesignExtractor::processAdvancedUsesAndModifies(std::vector<std::string> sortedProcedures) {
	for (std::string procedure : sortedProcedures) {
		processUsesCalls(procedure);
		processModifiesCalls(procedure);
		processUsesContainers(procedure);
		processModifiesContainers(procedure);
		processUsesProcedures(procedure);
		processModifiesProcedures(procedure);
	}
}

/*
 * Populates the Uses of Call Statements in SIMPLE
 */
void DesignExtractor::processUsesCalls(std::string procedure) {
	std::vector<int> procedureStm = pkb.getStmList(procedure);

	for (int i = 0; i < procedureStm.size() ; i++) {
		int currLine = procedureStm.at(i);
		stmType type = pkb.getStmType(currLine);

		if (type == stmType::call) {
			std::string procedure = pkb.getProcCalledBy(currLine);
			std::unordered_set<std::string> usedVars = pkb.getVarUsedByProc(procedure);

			for (std::string var : usedVars) {
				pkb.addUsesStm(currLine, var);
			}
		}
	}
}

/*
 * Populates the Uses of Modifies Statements in SIMPLE
 */
void DesignExtractor::processModifiesCalls(std::string procedure) {
	std::vector<int> procedureStm = pkb.getStmList(procedure);

	for (int i = 0; i < procedureStm.size(); i++) {
		int currLine = procedureStm.at(i);
		stmType type = pkb.getStmType(currLine);

		if (type == stmType::call) {
			std::string procedure = pkb.getProcCalledBy(currLine);
			std::unordered_set<std::string> modifiedVars = pkb.getVarModifiedByProc(procedure);

			for (std::string var : modifiedVars) {
				pkb.addModifiesStm(currLine, var);
			}
		}
	}
}

/*
 * Processes the the variables that are MODIFIED in Containing Statement so that the While/If Statement
 * also MODIFY these variables.
 */
void DesignExtractor::processModifiesContainers(std::string procedure) {
	//TODO: Implement for Iteration 2
	std::vector<int> procedureStm = pkb.getStmList(procedure);

	for (int i = procedureStm.size() - 1; i >= 0; i--) {
		int currLine = procedureStm.at(i);
		unordered_set<int> descendents = pkb.getDescendants(currLine);
		for (int descendent : descendents) {
			unordered_set<string> modifiedList = pkb.getVarModifiedByStm(descendent);
			for (string modifiedVar : modifiedList) {
				pkb.addModifiesStm(currLine, modifiedVar);
			}
		}
	}
}

/*
 * Processes the the variables that are USED in Containing Statement so that the While/If Statement
 * also USE these variables.
 */
void DesignExtractor::processUsesContainers(std::string procedure) {
	//TODO: Implement for Iteration 2
	std::vector<int> procedureStm = pkb.getStmList(procedure);

	for (int i = procedureStm.size() - 1; i >= 0; i--) {
		int currLine = procedureStm.at(i);
		unordered_set<int> descendents = pkb.getDescendants(currLine);
		for (int descendent : descendents) {
			unordered_set<string> usedList = pkb.getVarUsedByStm(descendent);
			for (string usedVariable : usedList) {
				pkb.addUsesStm(currLine, usedVariable);
			}
		}
	}
}

/*
 * Processes the variables that are USED in a procedure, by checking the use Relation for all Statements belonging
 * to that procedure.
 */
void DesignExtractor::processUsesProcedures(std::string procedure)
{
	//TODO: Implement for Iteration 2
	std::vector<int> procedureStm = pkb.getStmList(procedure);

	for (int i = 0; i < procedureStm.size() ; i++) {
		int currLine = procedureStm.at(i);
		std::unordered_set<std::string> usedList = pkb.getVarUsedByStm(currLine);

		for (std::string usedVar : usedList) {
			pkb.addUsesProc(procedure, usedVar);
		}
	}

}

/*
 * Processes the variables that are MODIFIED in a procedure, by checking the modifying Relation for all Statements belonging
 * to that procedure.
 */
void DesignExtractor::processModifiesProcedures(std::string procedure)
{
	//TODO: Implement for Iteration 2
	std::vector<int> procedureStm = pkb.getStmList(procedure);

	for (int i = 0; i < procedureStm.size(); i++) {
		int currLine = procedureStm.at(i);
		std::unordered_set<std::string> modifiedList = pkb.getVarModifiedByStm(i);

		for (std::string modifiedVar : modifiedList) {
			pkb.addModifiesProc(procedure, modifiedVar);
		}
	}
}

/*
 * Processes the Call* Relationship
 */
void DesignExtractor::processCallsStar(std::vector<std::string> sortedProcedures) {
	//Process procedure list s where Calls*(s, procedure) is true
	for (int i = sortedProcedures.size - 1; i >= 0; i++) {
		std::string procedure = sortedProcedures.at(i);
		std::unordered_set<std::string> callerList = pkb.getCaller(procedure);

		std::unordered_set<std::string> allAncestorCallers;

		//If Someone called this procedure
		if (!callerList.empty()) {
			for (std::string caller : callerList) {
				std::unordered_set<std::string> ancestorCallersList = pkb.getCallAnc(caller);
				for (std::string ancestorCaller : ancestorCallersList) {
					allAncestorCallers.insert(ancestorCaller);
				}
				allAncestorCallers.insert(caller);
			}
			pkb.setCallAnc(procedure, allAncestorCallers);
		}
	}
	
	//Process procedure list s where Calls*(procedure, s) is true.
	for (int i = 0; i < sortedProcedures.size; i++) {
		std::string procedure = sortedProcedures.at(i);
		std::unordered_set<std::string> calleeList = pkb.getCallee(procedure);

		std::unordered_set<std::string> allDescendentsCallee;

		//If this procedure called something.
		if (!calleeList.empty()) {
			for (std::string callee : calleeList) {
				std::unordered_set<std::string> descendentsCalleeList = pkb.getCallDesc(callee);
				for (std::string descendentCallee : descendentsCalleeList) {
					allDescendentsCallee.insert(descendentCallee);
				}
				allDescendentsCallee.insert(callee);
			}
			pkb.setCallDesc(procedure, allDescendentsCallee);
		}
	}
}