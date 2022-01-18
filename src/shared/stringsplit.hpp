#ifndef WS_SHARED_STRINGSPLIT_HPP
# define WS_SHARED_STRINGSPLIT_HPP

# include <string>
# include <vector>

namespace ws
{
	namespace shared
	{
		std::vector<std::string> stringsplit(std::string input, std::string delim);
	}
}

#endif
