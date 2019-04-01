#pragma once
#include <unordered_set>
#include <unordered_map>


#include "Hasher.h"

class ControlVariableStorage {

public:
	ControlVariableStorage();
	void addIfControlVariable(int stm, string variable);
	void addWhileControlVariable(int stm, string variable);

	//Call for if(_,_)
	bool hasIfControlVariable();

	//Call for w(_,_)
	bool hasWhileControlVariable();

	//Call for if('s',_)
	std::unordered_set<int> getIfStmWithControlVariable(std::string variable);

	//Call for w('s',_)
	std::unordered_set <int> getWhileStmWithControlVariable(std::string variable);

	//Call for if(s,_)
	std::unordered_set<std::pair<int, std::string>, intStringhash> getIfStmControlVariablePair();

	//Call for w(s,_)
	std::unordered_set<std::pair<int, std::string>, intStringhash> getWhileStmControlVariablePair();

private:
	static std::unordered_map<std::string, std::unordered_set<int>> ifVariableToStm;
	static std::unordered_map<std::string, std::unordered_set<int>> whileVariableToStm;
};