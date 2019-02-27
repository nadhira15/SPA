#pragma once

#include <functional>
#include <utility>

using namespace std;

// These self-defined hashers use Szudzik's function found in 
//https://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way
struct intPairhash {
public:
	template <typename T, typename U>
	size_t operator()(const pair<T, U> &x) const
	{
		T a = x.first;
		U b = x.second;
		return a >= b ? a * a + a + b : a + b * b;
	}
};

struct intStringhash {
public:
	template <typename T, typename U>
	size_t operator()(const pair<T, U> &x) const
	{
		std::hash<std::string> strHasher;

		T a = x.first;
		auto b = strHasher(x.second);
		return a >= b ? a * a + a + b : a + b * b;
	}
};

struct strPairhash {
public:
	template <typename T, typename U>
	size_t operator()(const pair<T, U> &x) const
	{
		std::hash<std::string> strHasher;

		auto a = strHasher(x.first);
		auto b = strHasher(x.second);
		return a >= b ? a * a + a + b : a + b * b;
	}
};
