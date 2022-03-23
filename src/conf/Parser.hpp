#ifndef WS_CONF_PARSER_HPP
# define WS_CONF_PARSER_HPP
# define ERROR 0
# define SUCCESS 1
# include <string>
# include <fstream>
# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include "return_code.hpp"

# include "Config.hpp"

namespace ws
{
	namespace conf
	{
		class Parser
		{
			private:
				bool valid;
			public:
			Parser(std::string file_path = "./config/example.wsconf");
			~Parser();
			Config						to_config();
			void						set_default_parameter(server_map &server);
			void						print_config(server_map server);
			std::string					p_route(std::string line);
			host_port					map_servers(std::string line);
			std::string					p_root(std::string line);
			std::string					p_index(std::string line);
			bool						p_a_index(std::string line);
			int							p_m_bdy_size(std::string line);
			std::string					p_upload_path(std::string line);
			std::string					p_return_path(std::string line);
			std::string					p_return_code(std::string line);
			ErrorPages					p_error_pages(ErrorPages errors_pages, std::string line);
			std::vector<std::string>	p_server_names(std::string line);
			std::vector<e_method>		p_accpt_mtde(std::string line);
			std::string					p_cgi_pass(std::string line);
			std::string					p_cgi_ext(std::string line);
			int							mapping_servers(server_map &config, std::ifstream &fd);
			std::string					loc_attr(std::ifstream &fd, Server &server, std::string prev_line);
			Config config;
		};
		
	}
}

#endif
