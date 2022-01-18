#include "stringsplit.hpp"

namespace ws
{
	namespace shared
	{
		std::vector<std::string> stringsplit(std::string input, std::string delim)
		{
			std::vector<std::string> out;

			size_t start = 0;
			size_t end = input.find(delim);
			while (end != std::string::npos)
			{
				out.push_back(input.substr(start, end - start));
				start = end + delim.length();
				end = input.find(delim, start);
			}
			out.push_back(input.substr(start, end));

			return out;
		}
	}
}