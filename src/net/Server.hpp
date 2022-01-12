#ifndef WS_NET_SERVER_HPP
# define WS_NET_SERVER_HPP

# include <string>
# include <iostream>

# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

# include "Socket.hpp"

namespace ws
{
	namespace net
	{
		class Server
		{
		public:

			Server();
			~Server();

			void listen(std::string port);

			Socket accept();

		private:

			int _socketfd;

		};
	}
}

#endif
