#pragma once
#include <string>

/* Checking to test if the parts of such that clause contains a synonym */
bool isSuchThatSynonym(std::string str) {
	bool res = false;
	if (str.find('"') == std::string::npos) {
		if (!isNumber(str)) {
			if (str.compare("_") != 0) {
				res = true;
			}
		}
	}
	return res;
}

/* Checking to test if the parts of pattern clause contains a synonym */
bool isPatternSynonym(std::string str) {
	bool res = false;
	if (str.find('"') == std::string::npos) {
		if (str.compare("_") != 0) {
			res = true;
		}
	}
	return res;
}

/* Checking to test if the parts of with clause contains a synonym */
bool isWithSynonym(std::string str) {
	bool res = false;
	if (str.find('.') != std::string::npos) {
		res = true;
	}
	return res;
}

/* Checking if string is a number */
bool isNumber(std::string str) {
	return (str.find_first_not_of("0123456789") == std::string::npos);
}

// Used for getting PQ
int getSuchThatPriority(std::string relation, int synNum) {
	//Priority determined by the relation and number of synonyms
	int ans;
	if (relation.compare("Follows") == 0) {
		
	} else if (relation.compare("Follows*") == 0) {

	} else if (relation.compare("Modifies") == 0 || relation.compare("Uses") == 0) {

	} else if (relation.compare("Next") == 0) {

	} else if (relation.compare("Next*") == 0) {

	} else if (relation.compare("Calls") == 0) {

	} else if (relation.compare("Calls*") == 0) {

	} else if (relation.compare("Affects") == 0) {

	} else if (relation.compare("Affects*") == 0) {

	}
	return ans;
}

int getWithPriority(std::string relation, int synNum) {
	int ans;
	//determine with priority by? what is required as inputs?
	return ans;
}

int getPatternPriority(std::string synonym, int synNum) {
	int ans;
	//determine with priority by? what is required as inputs?
	return ans;
}
