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

			Pool(std::list<Server> srv);
			~Pool();

			std::list<Connection> probe();

		private:

			fd_set _set;
			int _fdmax;
			std::list<Server> _srv;
			std::list<Connection> _con;

		};
	}
}


#endif
