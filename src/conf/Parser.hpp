#ifndef WS_CONF_PARSER_HPP
# define WS_CONF_PARSER_HPP

# include <string>
# include <iostream>
# include <fstream>
# include <vector>

# include "Config.hpp"

namespace ws
{
	namespace conf
	{
		class Parser
		{
		public:

			Parser(std::string path);
			~Parser();

		private:
			Config _conf;
		};
	}
}

#endif