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
			public:
			Parser(std::string file_path = "../../config/example.wsconf");
			~Parser();

			Config to_config();
			Config config;
		};
		std::string p_route(std::string line);
		host_port map_servers(std::string line);
		std::string p_root(std::string line);
		std::string p_index(std::string line);
		bool p_a_index(std::string line);
		int p_m_bdy_size(std::string line);
		std::string p_upload_path(std::string line);
		std::string p_return_path(std::string line);
		int p_return_code(std::string line);
		void  mapping_servers(server_map config, std::ifstream &fd);
	}
}

#endif