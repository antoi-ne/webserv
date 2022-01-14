#ifndef WS_NET_CONNECTION_HPP
# define WS_NET_CONNECTION_HPP

# include <sys/socket.h>

# include "shared/Buffer.hpp"
# include "Socket.hpp"

namespace ws
{
	namespace net
	{
		class Connection: public Socket
		{
		public:

			void send(shared::Buffer buff);

			shared::Buffer recv(size_t size);

		private:

		};
	}
}

#endif
