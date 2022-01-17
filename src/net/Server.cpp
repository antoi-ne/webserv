#include "Server.hpp"

namespace ws
{
	namespace net
	{
		Server::Server(in_port_t port)
			: _port(port)
		{
			struct addrinfo hints, *servinfo, *p;
			int yes = 1;

			std::memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			if (::getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &servinfo) < 0)
				shared::Log::fatal("net::Server: syscall getaddrinfo failed");

			for (p = servinfo; p != NULL; p = p->ai_next)
			{
				if ((this->_sockfd = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
					continue;

				if (::setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
					shared::Log::fatal("net::Server: syscall setsockopt failed");

				if (::bind(this->_sockfd, p->ai_addr, p->ai_addrlen) < 0)
				{
					::close(this->_sockfd);
					continue;
				}

				break;
			}

			freeaddrinfo(servinfo);

			if (p == NULL)
				shared::Log::fatal("net::Server: failed to bind");
		}

		void Server::listen(int backlog)
		{
			if (::listen(this->_sockfd, backlog) < 0)
				shared::Log::fatal("net::Server: syscall listen failed");
		}

		Connection Server::accept()
		{
			int cs = ::accept(this->_sockfd, NULL, NULL);

			return Connection(cs);
		}

		in_port_t Server::get_port()
		{
			return this->_port;
		}
	}
}
