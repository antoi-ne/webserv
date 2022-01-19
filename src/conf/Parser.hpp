#ifndef WS_CONF_PARSER_HPP
# define WS_CONF_PARSER_HPP

# include <string>

# include "Config.hpp"

namespace ws
{
	namespace conf
	{
		class Parser
		{
			Parser(std::string file_path = "./webserv.conf");
			~Parser();

			Config to_config();
		};
	}
}

#endif
