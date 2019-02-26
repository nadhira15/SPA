#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <sstream>

namespace StringUtil
{
	std::vector<std::string> split(std::string stmt, char delimiter);

	std::string trim(std::string& str, std::string& whitespace);
}