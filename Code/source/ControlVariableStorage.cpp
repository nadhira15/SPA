#include "ControlVariableStorage.h"

std::unordered_map<std::string, std::unordered_set<int>> ControlVariableStorage::ifVariableToStm;
std::unordered_map<std::string, std::unordered_set<int>> ControlVariableStorage::whileVariableToStm;

ControlVariableStorage::ControlVariableStorage() {

}

void ControlVariableStorage::addIfControlVariable(int stm, string variable) {

	//if cannot find key
	if (ifVariableToStm.find(variable) == ifVariableToStm.end()) {
		std::unordered_set<int> stmList;
		stmList.insert(stm);
		ifVariableToStm[variable] = stmList;
	} else {
		std::unordered_set<int> stmList = ifVariableToStm[variable];
		stmList.insert(stm);
		ifVariableToStm[variable] = stmList;
	}
}

void ControlVariableStorage::addWhileControlVariable(int stm, string variable) {

	//If cannot find key
	if (whileVariableToStm.find(variable) == whileVariableToStm.end()) {
		std::unordered_set<int> stmList;
		stmList.insert(stm);
		whileVariableToStm[variable] = stmList;
	} else {
		std::unordered_set<int> stmList = whileVariableToStm[variable];
		stmList.insert(stm);
		whileVariableToStm[variable] = stmList;
	}
}

bool ControlVariableStorage::hasIfControlVariable() {

	if (ifVariableToStm.empty()) {
		return false;
	} else {
		return true;
	}
}

bool ControlVariableStorage::hasWhileControlVariable() {

	if (whileVariableToStm.empty()) {
		return false;
	} else {
		return true;
	}
}

std::unordered_set<int> ControlVariableStorage::getIfStmWithControlVariable(std::string variable) {

	return ifVariableToStm[variable];

}

std::unordered_set<int> ControlVariableStorage::getWhileStmWithControlVariable(std::string variable) {

	return whileVariableToStm[variable];

}

std::unordered_set<std::pair<int, std::string>, intStringhash> ControlVariableStorage::getIfStmControlVariablePair() {

	std::unordered_set<std::pair<int, std::string>, intStringhash> result;

	for (std::pair<std::string, std::unordered_set<int>> pair : ifVariableToStm) {
		for (int stm : pair.second) {
			result.insert(std::make_pair(stm, pair.first));
		}
	}
	return result;
}

std::unordered_set<std::pair<int, std::string>, intStringhash> ControlVariableStorage::getWhileStmControlVariablePair()
{
	std::unordered_set<std::pair<int, std::string>, intStringhash> result;

	for (std::pair<std::string, std::unordered_set<int>> pair : whileVariableToStm) {
		for (int stm : pair.second) {
			result.insert(std::make_pair(stm, pair.first));
		}
	}
	return result;
}

