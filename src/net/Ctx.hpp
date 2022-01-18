#ifndef WS_NET_CTX_HPP
# define WS_NET_CTX_HPP

# include "Connection.hpp"
# include "Server.hpp"

namespace ws
{
	namespace net
	{
		struct Ctx
		{
			Connection con;
			Server srv;
			bool read;
		};
	}
}

#endif
