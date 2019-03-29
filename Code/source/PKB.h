#pragma once

#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

#include "FollowStorage.h"
#include "ParentStorage.h"
#include "UseStorage.h"
#include "ModifyStorage.h"
#include "CallStorage.h"
#include "NextStorage.h"
#include "Hasher.h"

enum stmType { read, print, assign, whileStm, ifStm, call};

/*
	Accepts relationship, pattern and other general data from Parser and DesignExtractor and
	stores them here or into their respective Storage classes.
	Reply to queries made by the QueryEvaluator.
*/
class PKB {
public:
	PKB();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//General adder Methods			/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// add a procedure to procList
	void addProc(string procName);

	//add statement to its respective StmList and set stmTypeList[stmNo] to type 
	void addStatement(int stmNo, stmType type);

	//add statement to its respective StmLists and set stmTypeList[stmNo] to type 
	void addStatement(int stmNo, stmType type, string procedure);

	// add variable to varList
	void addVariable(string name);

	// add constant to constList
	void addConstant(string value);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Follows adder & setter Methods	/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		add the follows relation in FollowStorage
		Returns false if
			1) the pair is already stored
			2) the followed statement has another follower stored
			3) the follower is following another statement
			4) stm2 <= stm1 or stm1, stm2 <= 0
	*/
	bool addFollow(int stm1, int stm2);

	/*
		Sets the list of followers of 'stm' to be 'stmList' in FollowStorage
		Every FollowStar pair is stored as well
		If stm already has a list of followers, it is not replaced and it return false
	*/
	bool setAllFollowing(int stm, unordered_set<int> stmList);

	/*
		Sets the list of followed of 'stm' to be 'stmList' in FollowStorage
		Every FollowStar pair is stored as well
		If stm already has a list of followed, it is not replaced and it return false
	*/
	bool setAllFollowedBy(int stm, unordered_set<int> stmList);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Parent adder & setter Methods		/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		Adds the parent relation into ParentStorage
		Returns false if	
			1) the pair is already stored
			2) the child already has another parent
			3) stm2 <= stm1 or stm1, stm2 <= 0
	*/
	bool addParent(int stm1, int stm2);

	/*
		Sets the list of ancestors of 'stm' in ParentStorage
		Each Parent* pair is stored as well
		If stm already has a list of ancestors, it is not replaced and it return false
	*/
	bool setAncestors(int stm, unordered_set<int> stmList);

	/*
		Sets the list of descendants of 'stm' in ParentStorage
		Each Parent* pair is stored as well
		If stm already has a list of descendants, it is not replaced and it return false
	*/
	bool setDescendants(int stm, unordered_set<int> stmList);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Uses adder Methods			/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		add the Uses relation for a Statement in UseStorage
		Returns false if
			1) the pair is already stored
			2) stm <= 0 and variable is an empty string
	*/
	bool addUsesStm(int stm, string variable);

	/*
		add the Uses relation for a Procedure in UseStorage
		Returns false if
			1) the pair is already stored
			2) procedure or variable is an empty string
	*/
	bool addUsesProc(string procedure, string variable);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Modifies adder Methods			/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		add the Modifies relation for a Statement in UseStorage
		Returns false if
			1) the pair is already stored
			2) stm <= 0 and variable is an empty string
	*/
	bool addModifiesStm(int stm, string variable);

	/*
		add the Modifies relation for a Procedure in UseStorage
		Returns false if
			1) the pair is already stored
			2) procedure or variable is an empty string
	*/
	bool addModifiesProc(string procedure, string variable);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Calls adder & setter Methods	/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		Adds the call relation into CallStorage
		Returns false if
			1) the pair is already stored
			2) proc1 or proc2 == ""
	*/
	bool addCall(string proc1, string proc2);

	/*
		Adds the call relation at statement 'stmNo' into CallStorage
		Returns false if
			1) the pair is already stored
			2) proc1 or proc2 == ""
			3) stmNo <= 0
	*/
	bool addCall(string proc1, string proc2, int stmNo);

	/*
		Sets the list of call ancestors of 'procedure' in CallStorage
		Each Call* pair is stored as well
		If 'procedure' already has a list of call ancestors, it is not replaced and it return false
	*/
	bool setCallAnc(string procedure, unordered_set<string> procList);

	/*
		Sets the list of call descendants of 'procedure' in CallStorage
		Each Call* pair is stored as well
		If 'procedure' already has a list of call descendants, it is not replaced and it return false
	*/
	bool setCallDesc(string procedure, unordered_set<string> procList);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Next adder Methods		/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		add the next relation in NextStorage
		Returns false if
			1) the pair is already stored
			2) the previous program line has another next program line stored
			3) the next program line has another previous program line stored
			4) line2 <= line1 or line1, line2 <= 0
	*/
	bool addNext(int line1, int line2);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Pattern adder Methods			/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		add a pattern for an assign statement to patternList
		returns false if 'stm' already exist in the list with another pattern
	*/
	bool addAssignPattern(int stm, string variable, string expr);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//General Getter Methods	/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// returns the stored list of procedures
	unordered_set<string> getProcList();

	// returns an ordered list of statements that are in 'procedure'
	vector<int> getStmList(string procedure);

	// returns the total number of statements in the entire program
	int getTotalStmNo();

	// return the statement type of stm
	stmType getStmType(int stm);

	// returns the stored list of read statements
	unordered_set<int> getReadStms();

	// returns the stored list of print statements
	unordered_set<int> getPrintStms();

	// returns the stored list of assign statements
	unordered_set<int> getAssignStms();

	// returns the stored list of if statements
	unordered_set<int> getIfStms();

	// returns the stored list of while statements
	unordered_set<int> getWhileStms();

	// returns the stored list of call statements
	unordered_set<int> getCallStms();

	// returns the stored list of variables
	unordered_set<string> getVariables();

	// returns the stored list of constants
	unordered_set<string> getConstants();

	/*
		returns the procedure called by 'stm'
		returns empty string if 'stm' is not found
	*/
	string getProcCalledBy(int stm);

	/*
		returns a list of statements that calls 'procedure'
		returns an empty set {} if 'procedure' is not found
	*/
	unordered_set<int> getStmCalling(string procedure);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Follows Getter Methods	/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if there is at least one follows relation
	bool hasFollowRelation();

	// checks if the relation Follows*(stm1, stm2) exist
	bool hasFollowStarPair(int stm1, int stm2);

	/*
		return the statement followed by 'stm'
		return 0 if 'stm' is not found
	*/
	int getStmFollowedBy(int stm);

	/*
		return the statement following 'stm'
		return 0 if 'stm' is not found
	*/
	int getFollower(int stm);

	/*
		return a list of statements that is directly/indirectly following 'stm'
		return an empty set if 'stm' is not found
	*/
	unordered_set<int> getAllFollowing(int stm);

	/*
		return a list of statements that is directly/indirectly followed by 'stm'
		return an empty set if 'stm' is not found
	*/
	unordered_set<int> getAllFollowedBy(int stm);

	// returns a list of all statements that follows another
	unordered_set<int> getAllFollowers();

	// returns a list of all statements that is followed by another
	unordered_set<int> getAllFollowed();

	// returns a list of all follows pairs
	unordered_set< pair<int, int>, intPairhash> getFollowPairs();

	// returns a list of all follows* pairs
	unordered_set< pair<int, int>, intPairhash> getFollowStarPairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Parent Getter Methods		/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if there is at least one Parent relationship
	bool hasParentRelation();

	// checks if 'stm' is a parent of another
	bool isParent(int stm);

	// checks if 'stm' is a child of another
	bool isChild(int stm);

	// checks if the relation Parent*(stm1, stm2) exist
	bool hasAncDescPair(int stm1, int stm2);

	/*
		return a statement that is the parent of 'stm'
		return 0 if 'stm' is not found
	*/
	int getParent(int stm);

	/*
		return a list of statements that is the children of 'stm'
		return an empty set if 'stm' is not found
	*/
	unordered_set<int> getChildren(int stm);

	/*
		return a list of statements that is the ancestors of 'stm'
		return an empty set if 'stm' is not found
	*/
	unordered_set<int> getAncestors(int stm);

	/*
		return a list of statements that is the descendants of 'stm'
		return an empty set if 'stm' is not found
	*/
	unordered_set<int> getDescendants(int stm);

	// returns a list of all statements that is the parent of another
	unordered_set<int> getAllParents();

	// returns a list of all statements that is the child of another
	unordered_set<int> getAllChildren();

	// returns a list of all parent pairs
	unordered_set< pair<int, int>, intPairhash> getParentChildPairs();

	// returns a list of all parent* pairs
	unordered_set< pair<int, int>, intPairhash> getAncDescPairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Uses Getter Methods		/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if the relation Uses(stm, variable) exist
	bool isStmUsing(int stm, string variable);

	// checks if the relation Uses(procedure, variable) exist
	bool isProcUsing(string procedure, string variable);

	/*
		return a list of variables that is used by 'stm'
		return an empty set if 'stm' is not found
	*/
	unordered_set<string> getVarUsedByStm(int stm);

	/*
		return a list of variables that is used by 'procedure'
		return an empty set if 'procedure' is not found
	*/
	unordered_set<string> getVarUsedByProc(string procedure);

	/*
		return a list of statements that is using 'variable'
		returns a list of all statements that used a variable if 'variable' == "" 
		return an empty set if 'variable' is not found
	*/
	unordered_set<int> getStmUsing(string variable);

	/*
		return a list of procedure that is using 'variable'
		returns a list of all procedures that used a variable if 'variable' == "" 
		return an empty set if 'variable' is not found
	*/
	unordered_set<string> getProcUsing(string variable);

	// returns a list of all Uses pairs for statements
	unordered_set< pair<int, string>, intStringhash> getStmVarUsePairs();

	// returns a list of all Uses pairs for procedures
	unordered_set< pair<string, string>, strPairhash> getProcVarUsePairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Modifies Getter Methods	/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if the relation Modifies(stm, variable) exist
	bool isStmModifying(int stm, string variable);

	// checks if the relation Modifies(procedure, variable) exist
	bool isProcModifying(string procedure, string variable);

	/*
		return a list of variables that is modified by 'stm'
		return an empty set if 'stm' is not found
	*/
	unordered_set<string> getVarModifiedByStm(int stm);

	/*
		return a list of variables that is modified by 'procedure'
		return an empty set if 'procedure' is not found
	*/
	unordered_set<string> getVarModifiedByProc(string procedure);

	/*
		return a list of statements that modifies 'variable'
		returns a list of all statements that modifies a variable if 'variable' == "" 
		return an empty set if 'variable' is not found
	*/
	unordered_set<int> getStmModifying(string variable);

	/*
		return a list of procedures that modifies 'variable'
		returns a list of all procedures that modifies a variable if 'variable' == "" 
		return an empty set if 'variable' is not found
	*/
	unordered_set<string> getProcModifying(string variable);

	// returns a list of all Modifies pairs for statements
	unordered_set< pair<int, string>, intStringhash> getStmVarModifyPairs();

	// returns a list of all Modifies pairs for procedures
	unordered_set< pair<string, string>, strPairhash> getProcVarModifyPairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Call Getter Methods		/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if there is at least one Call relationship
	bool hasCallRelation();

	// checks if 'procedure' calls another
	bool isCaller(string proc);

	// checks if 'procedure' is called by another
	bool isCallee(string procedure);

	// checks if the relation Call*(proc1, proc2) exist
	bool hasCallStarPair(string proc1, string proc2);

	/*
		return the procedure calling 'procedure'
		return "" if 'procedure' is not found
	*/
	unordered_set<string> getCaller(string procedure);

	/*
		return the procedure called by 'procedure'
		return {} if 'procedure' is not found
	*/
	unordered_set<string> getCallee(string procedure);

	/*
		return a list of procedures that is directly/indirectly calling 'procedure'
		return an empty set if 'procedure' is not found
	*/
	unordered_set<string> getCallAnc(string procedure);

	/*
		return a list of procedures that is directly/indirectly called by 'procedure'
		return an empty set if 'procedure' is not found
	*/
	unordered_set<string> getCallDesc(string procedure);

	// returns a list of all procedures that calls another
	unordered_set<string> getAllCallers();

	// returns a list of all procedures that is called by another
	unordered_set<string> getAllCallees();

	// returns a list of all call pairs
	unordered_set< pair<string, string>, strPairhash> getCallPairs();

	// returns a list of all call* pairs
	unordered_set< pair<string, string>, strPairhash> getCallStarPairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Next Getter Methods	/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// checks if there is at least one next relation
	bool hasNextRelation();

	// checks if the relation Next*(line1, line2) exist
	bool hasNextStarPair(int line1, int line2);

	/*
		return the program line to be executed after 'line'
		return 0 if 'line' is not found
	*/
	int getNext(int line);

	/*
		return the program line to be executed before 'line'
		return 0 if 'line' is not found
	*/
	int getPrev(int line);

	/*
		return a list of program lines that is directly/indirectly after 'line'
		return an empty set if 'line' is not found
	*/
	unordered_set<int> getAllLnAfter(int line);

	/*
		return a list of program lines that is directly/indirectly before 'line'
		return an empty set if 'line' is not found
	*/
	unordered_set<int> getAllLnBefore(int line);

	// returns a list of all program lines that is executed after another
	unordered_set<int> getAllNext();

	// returns a list of all program lines that is before after another
	unordered_set<int> getAllPrev();

	// returns a list of all next pairs
	unordered_set< pair<int, int>, intPairhash> getNextPairs();

	// returns a list of all next* pairs
	unordered_set< pair<int, int>, intPairhash> getNextStarPairs();

/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Pattern Getter Methods	/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		Search for assign statements with pattern matching
			- the specified variable ont the left side and
			- the specified expression on the right side
		If isExclusive is true, the function finds for an exact match to the specfied expression.
		If false, it finds for a matching substring instead.
	*/
	unordered_set<int> findPattern(string variable, string expr, bool isExclusive);

	/*
		Search for assign statements with pattern matching
			- the specified expression on the right side
		If isExclusive is true, the function finds for an exact match to the specfied expression.
		If false, it finds for a matching substring instead.
	*/
	unordered_set<int> findPattern(string expr, bool isExclusive);

	/*
		Search for pairs of an assign statement and left-hand side variable with
			the right side expression matching the specified expression
		If isExclusive is true, the function finds for an exact match to the specfied expression.
		If false, it finds for a matching substring instead.
	*/
	unordered_set<pair<int, string>, intStringhash> findPatternPairs(string expr, bool isExclusive);

private:
	static unordered_set<string> procList;
	static unordered_map<string, vector<int>> procStmList;
	static vector<stmType> stmTypeList;
	static unordered_set<string> varList;
	static unordered_set<string> constList;
	static unordered_set<int> readStmList;
	static unordered_set<int> printStmList;
	static unordered_set<int> assignStmList;
	static unordered_set<int> ifStmList;
	static unordered_set<int> whileStmList;
	static unordered_set<int> callStmList;

	static FollowStorage fStore;
	static ParentStorage pStore;
	static UseStorage uStore;
	static ModifyStorage mStore;
	static CallStorage cStore;
	static NextStorage nStore;
	static unordered_map<int, pair<string, string> > patternList;
};