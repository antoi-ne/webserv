#ifndef WS_NET_CONNECTION_HPP
# define WS_NET_CONNECTION_HPP

# include <iostream>
# include <string>

# include <sys/socket.h>
# include "arpa/inet.h"

# include "shared/Buffer.hpp"
# include "shared/Option.hpp"
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

			bool send(shared::Buffer buff);

			shared::Option<shared::Buffer> recv(size_t size);

			bool operator==(const Connection& rhs) const;
			bool operator<(const Connection& rhs) const;

		};
	}
}

#endif
