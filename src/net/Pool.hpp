#ifndef WS_NET_POOL_HPP
# define WS_NET_POOL_HPP

# include <list>
# include <utility>

# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>

# include "Server.hpp"
# include "Connection.hpp"

namespace ws
{
	namespace net
	{
		class Pool
		{
		public:

			Pool();
			~Pool();

			void add_server(Server srv);
			void add_connection(Connection con);

			void probe();

		private:

			std::list<Server> _srv;
			std::list<Connection> _con;

		};
	}
}


#endif
