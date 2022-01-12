#include "Server.hpp"

namespace ws
{
	namespace net
	{

		Server::Server()
			: _socketfd(-1)
		{}

		Server::~Server()
		{
			if (this->_socketfd > 0)
				close(this->_socketfd);
		}

		void Server::listen(std::string port)
		{
			struct addrinfo hints, *serv_info, *p;
			int status;

			std::memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;

			if ((status = ::getaddrinfo(NULL, port.c_str(), &hints, &serv_info)) != 0)
			{
				std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
			}

			for (p = serv_info; p != NULL; p = p->ai_next)
			{
				if ((this->_socketfd = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
				{
					std::perror("socket");
					continue;
				}

				int yes = 1;

				if (setsockopt(this->_socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
					perror("setsockopt");
					exit(1);
				}

				if ((::bind(this->_socketfd, p->ai_addr, p->ai_addrlen)) == -1)
				{
					::close(this->_socketfd);
					this->_socketfd = -1;
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

			if (::listen(this->_socketfd, 10))
			{
				std::perror("listen");
				exit(1);
			}
		}

		Socket Server::accept()
		{
			int sfd;
			struct sockaddr_storage sin_addr;
			socklen_t sin_size;

			sin_size = sizeof(sin_addr);
			sfd = ::accept(this->_socketfd, (struct sockaddr *)&sin_addr, &sin_size);

			return Socket(sfd);
		}

	}
}