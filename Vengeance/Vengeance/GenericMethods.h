#pragma once
#include <sstream>
#include <iostream>
#include <vector>
#include <ctime>

namespace GenericMethods
{
	// Changes all characters in 's' to lowercase.
	std::string toLower(std::string s);

	// Whitespace trimming functions.
	std::string ltrim(std::string s);
	std::string rtrim(std::string s);
	std::string trim(std::string s);

	// Splits string 's' into tokens based on all characters in string 'delimiters'.
	std::vector<std::string> split(std::string s, std::string delimiters, bool shouldTrim = true);

	// Methods for converting between string and int.
	int stringToInt(std::string s);
	bool isStringValidInt(std::string s);
	std::string intToString(int i);

	// Searches toSearch for all occurrences of toFind and replaces with toReplace.
	std::string replace(std::string toSearch, char toFind, char toReplace, bool caseSensitive = false);
	std::string replace(std::string toSearch, std::string toFind, std::string toReplace, bool caseSensitive = false);

	// Returns a random int between lowerBound and upperBound.
	int rollNumber(int lowerBound, int upperBound);
}