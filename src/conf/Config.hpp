#ifndef WS_CONF_CONFIG_HPP
# define WS_CONF_CONFIG_HPP

# include <map>
# include <vector>
# include <string>
# include <utility>
# include "../http/method.h"
# include "../http/CI_Less.hpp"

namespace ws
{
	namespace conf
	{
		struct Location
		{
			std::string root;
			std::string index;
			std::vector<e_method> accepted_methods;
			bool autoindex; // false by default
			int max_body_size; // negative means not defined
			std::map<unsigned int,std::string> error_pages;
			std::string upload_path; // empty string means no upload path
			std::string return_path; // empty string means no return path
			int return_code; // negative means no return code
			std::string cgi_path;
		};

		// first route (path of the location) -> Location obj
		typedef std::map<std::string, Location, CI_Less>	location_map;

		struct Server
		{
			std::vector<std::string> server_names;
			std::string root;
			std::string index;
			bool autoindex;
			int max_body_size;
			std::map<unsigned int, std::string> error_pages;
			std::string upload_path;
			std::string return_path;
			int return_code;
			location_map locations;
		};

		typedef std::pair<std::string, uint16_t>			host_port;
		typedef std::map<host_port, std::vector<Server> >	server_map;

		struct Config
		{
			server_map servers;
		};

		typedef std::map<std::pair< std::string, uint16_t>, std::vector<Server> > server_map;
	}
}

#endif
