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
			bool rread;
			bool rwrite;

			Ctx();
			Ctx(Connection con, Server srv, bool rread = false, bool rwrite = false);
			~Ctx();
		};
	}
}

#endif
