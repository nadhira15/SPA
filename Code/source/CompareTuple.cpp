#pragma once
#include "tuple"

class CompareTuple {
	bool operator()(std::tuple<int, int, int> c1, std::tuple<int, int, int> c2) {
		return std::get<0>(c1) < std::get<0>(c2);
	}
};
