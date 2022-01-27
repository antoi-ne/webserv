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
		typedef std::map<unsigned int, const std::string*> ErrorPages;

		struct ServConfig 
		{
			std::string	route;
			std::string root;
			std::string index;
			bool autoindex; // false by default
			int max_body_size; // negative means not defined
			ErrorPages error_pages;
			std::string upload_path; // empty string means no upload path
			std::string return_path; // empty string means no return path
			int return_code;
		};

		struct Location : public ServConfig 
		{
			std::vector<e_method>	accepted_methods;
		};

		// first route (path of the location) -> Location obj
		typedef std::vector<Location>	location_v;

		struct Server : public ServConfig
		{
			std::vector<std::string>	server_names;
			location_v 					locations;
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
