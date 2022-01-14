#ifndef WS_NET_SOCKET_HPP
# define WS_NET_SOCKET_HPP

# include <unistd.h>

namespace ws
{
	namespace net
	{
		class Socket
		{
		public:

			Socket(int fd = -1);
			virtual ~Socket();

			int get_fd();

			void close();

		protected:

			int _sockfd;

		};
	}
}

#endif
