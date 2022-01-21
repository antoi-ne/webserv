#ifndef WS_CONF_CONFIG_HPP
# define WS_CONF_CONFIG_HPP

# include <map>
# include <vector>
# include <string>
# include <utility>

namespace ws
{
	namespace conf
	{
		enum Methods
		{
			GET,
			POST,
			DELETE
		};

		struct Location
		{
			std::string route; // path of the location
			std::string root;
			std::string index;
			std::vector<Methods> accepted_methods;
			bool autoindex; // false by default
			int max_body_size; // negative means not defined
			std::map<unsigned int,std::string> error_pages;
			std::string upload_path; // empty string means no upload path
			std::string return_path; // empty string means no return path
			int return_code; // negative means no return code
		};

		struct Server
		{
			std::vector<std::string> server_names;
			std::string root;
			std::string index;
			bool autoindex;
			int max_body_size;
			std::map<unsigned int,std::string> error_pages;
			std::string upload_path;
			std::string return_path;
			int return_code;
			std::vector<Location> locations;
		};

		struct Config
		{
			std::map<std::pair< std::string, uint16_t>, std::vector<Server> > servers;
		};

		typedef std::map<std::pair< std::string, uint16_t>, std::vector<Server> > server_map;
	}
}

#endif
