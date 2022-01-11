#ifndef WS_NET_SERVER_HPP
#define WS_NET_SERVER_HPP

#include <string>
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

namespace ws
{
	namespace net
	{
		class Server
		{
		public:
			Server(std::string port, int backlog, void (*handler)(int));

			~Server();

			void listen();

		private:
			std::string _port;
			int _backlog;
			struct addrinfo *_serv_addr;
			int _socket;
			void (*_handler)(int);
		};
	}
}

#endif
