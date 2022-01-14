#ifndef WS_CONF_CONFG_HPP
# define WS_CONF_CONFG_HPP

# include <string>
# include <vector>
# include <map>

namespace ws
{
	namespace conf
	{

		struct Target
		{
			std::string host;
			uint16_t port;
		};

		struct Route
		{
			std::string location;
			std::vector<std::string> tokens;
		};

		struct Server
		{
			std::vector<Target> targets;
			std::vector<std::string> server_names;
			std::map<uint16_t, std::string> error_pages;
			std::string client_max_body_size;
		};

		struct Config
		{
			std::vector<Server> servers;
		};

	}
}

#endif
