#ifndef WS_CONFIG_HPP
# define WS_CONFIG_HPP

#include <list>
#include <string>

namespace ws
{
	struct config
	{
		std::list<server> servers;
	};

	struct server
	{
		std::list<uint16_t> ports;
		std::string host;
		std::list<std::string> server_names;
	};
	
}

#endif
