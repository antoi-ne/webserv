#ifndef WS_NET_POOL_HPP
# define WS_NET_POOL_HPP

# include <list>
# include <utility>
# include <iostream>

# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>

# include "Server.hpp"
# include "Connection.hpp"
# include "Ctx.hpp"

namespace ws
{
	namespace net
	{
		class Pool
		{
		public:

			Pool(std::list<Server> srv = std::list<Server>());
			~Pool();

			std::list<Ctx> probe();

			void close_con(Connection con);

		private:

			fd_set _set;
			int _fdmax;
			std::list<Server> _srv;
			std::list<std::pair<Connection, Server> > _con;

		};
	}
}


#endif
