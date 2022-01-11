#include "Server.hpp"
#include <cstring>
#include <iostream>
#include <cerrno>
#include <unistd.h>

namespace ws
{
	namespace net
	{
		Server::Server(std::string port, int backlog, void (*handler)(int))
			: _port(port), _backlog(backlog), _handler(handler)
		{
			struct addrinfo hints, *serv_info, *p;
			int status;

			std::memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			if ((status = getaddrinfo(NULL, this->_port.c_str(), &hints, &serv_info)) != 0)
			{
				std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
			}

			for (p = serv_info; p != NULL; p = p->ai_next)
			{
				if ((this->_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) != 0)
				{
					std::perror("socket");
					continue;
				}
				if ((bind(this->_socket, p->ai_addr, p->ai_addrlen)) == -1)
				{
					close(this->_socket);
					this->_socket = -1;
					continue;
				}
				break;
			}

			freeaddrinfo(serv_info);

			if (p == NULL)
			{
				std::cerr << "failed to bind" << std::endl;
				exit(1);
			}
		}

		Server::~Server()
		{
			if (this->_socket != -1)
				close(this->_socket);
		}

		void Server::listen()
		{
			if (::listen(this->_socket, this->_backlog))
			{
				std::perror("listen");
				exit(1);
			}
		}
	}
}
