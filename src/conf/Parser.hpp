#ifndef WS_CONF_PARSER_HPP
# define WS_CONF_PARSER_HPP
# define ERROR 0
# define SUCCESS 1
# include <string>
# include <fstream>
# include <iostream>

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
			Config config;
		};
	}
}

#endif
