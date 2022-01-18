#ifndef WS_NET_CONNECTION_HPP
# define WS_NET_CONNECTION_HPP

# include <iostream>
# include <string>

# include <sys/socket.h>
# include "arpa/inet.h"

# include "shared/Buffer.hpp"
# include "shared/Log.hpp"
# include "Socket.hpp"

namespace ws
{
	namespace net
	{
		class Connection: public Socket
		{
		public:

			Connection(int fd = -1);

			void send(shared::Buffer buff);

			shared::Buffer recv(size_t size);

			std::string get_address();

		};
	}
}

#endif
