#include "utils.hpp"

namespace ws
{
	namespace utils
	{
		std::vector<std::string> split(std::string input, char delim)
		{
			std::vector<std::string> output;
			std::stringstream stream(input);
			std::string token;

			while (std::getline(stream, token, delim))
				output.push_back(token);

			return output;
		}
	}
}
