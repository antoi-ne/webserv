#ifndef WS_NET_SOCKET_HPP
# define WS_NET_SOCKET_HPP

# include <exception>
# include <string>
# include <iostream>
# include <unistd.h>
# include <sys/socket.h>

namespace ws
{
	namespace net
	{
		class Socket
		{
		public:

			class SocketClosedException: public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

			Socket(int fd);
			~Socket();

			std::string recv(size_t size);
			void send(std::string str);

			void close();

		private:

			int _fd;
			bool _open;

		};
	}
}

#endif
