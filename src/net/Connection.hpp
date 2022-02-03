#ifndef WS_NET_CONNECTION_HPP
# define WS_NET_CONNECTION_HPP

# include <iostream>
# include <string>
# include <ctime>

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

			Connection(int fd = -1, std::time_t created_at = 0);

			ssize_t send(shared::Buffer buff);

			shared::Option<shared::Buffer> recv(size_t size);

			std::time_t created_at();

			// Connection & operator=(const connection & rhs);

			bool operator==(const Connection& rhs) const;
			bool operator<(const Connection& rhs) const;

		private:

			std::time_t _created_at;

		};
	}
}

#endif
