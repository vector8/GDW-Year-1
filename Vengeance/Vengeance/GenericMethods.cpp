#include "GenericMethods.h"
#include <assert.h>

namespace GenericMethods
{
	std::string toLower(std::string s)
	{
		for(unsigned i = 0; i < s.length(); i++)
		{
			s[i] = tolower(s[i]);
		}

		return s;
	}

	std::string ltrim(std::string s)
	{
		size_t startIndex;
		std::string result;

		if(s.size() == 0)
			result = s;
		else
		{
			startIndex = s.find_first_not_of(" \n\r\t");

			if(startIndex == std::string::npos)
				result = "";
			else
				result = s.substr(startIndex);
		}

		return result;
	}

	std::string rtrim(std::string s)
	{
		size_t endIndex;
		std::string result;

		if(s.size() == 0)
			result = s;
		else
		{
			endIndex = s.find_last_not_of(" \n\r\t");

			if(endIndex == std::string::npos)
				result = "";
			else
				result = s.substr(0, endIndex + 1);
		}

		return result;
	}

	std::string trim(std::string s)
	{
		return ltrim(rtrim(s));
	}

	std::vector<std::string> split(std::string s, std::string delimiters, bool shouldTrim)
	{
		std::vector<std::string> result;
		std::string temp;

		size_t start = 0;
		size_t end = s.find_first_of(delimiters);

		while(end != std::string::npos)
		{
			temp = s.substr(start, end - start);

			if(shouldTrim)
				temp = trim(temp);

			if(temp.size() > 0)
				result.push_back(temp);

			start = end + 1;
			end = s.find_first_of(delimiters, start);
		}

		// get the last token...
		temp = s.substr(start);

		if(shouldTrim)
			temp = trim(temp);
			
		if(temp.size() > 0)
			result.push_back(temp);

		return result;
	}

	int stringToInt(std::string s)
	{
		int result;

		std::stringstream ss(s);

		assert((ss >> result));

		return result;
	}

	bool isStringValidInt(std::string s)
	{
		std::stringstream ss(s);
		int val;

		if(ss >> val)
			return true;
		else
			return false;
	}

	std::string intToString(int i)
	{
		std::stringstream ss;

		ss << i;

		return ss.str();
	}

	std::string replace(std::string toSearch, char toFind, char toReplace, bool caseSensitive)
	{
		std::string result = toSearch;

		if(!caseSensitive)
		{
			toSearch = toLower(toSearch);
			toFind = tolower(toFind);
			toReplace = tolower(toReplace);
		}

		if(toFind == toReplace)
			return result;

		for(unsigned i = 0; i < toSearch.size(); i++)
		{
			if(toSearch[i] == toFind)
				result[i] = toReplace;
		}

		return result;
	}

	std::string replace(std::string toSearch, std::string toFind, std::string toReplace, bool caseSensitive)
	{
		std::string result = toSearch;
		int start, end;

		assert(toFind.size() > 0);

		if(!caseSensitive)
		{
			result = toLower(result);
			toFind = toLower(toFind);
			toReplace = toLower(toReplace);
		}

		if(toFind == toReplace)
			return result;

		start = result.find(toFind);

		while(start != std::string::npos)
		{
			end = start + toFind.size();

			result = result.substr(0, start) + toReplace + result.substr(end, result.size() - end + 1);

			start = result.find(toFind);
		}

		return result;
	}

	int rollNumber(int lowerBound, int upperBound)
	{
		assert(lowerBound >= 0);

		static bool first = true;

		if(first)
		{
			srand((unsigned) time(NULL));
			first = false;
		}

		return rand() % upperBound + lowerBound;
	}
}