#include "Socket.hpp"

namespace ws
{
	namespace net
	{
		Socket::Socket(int fd)
			: _sockfd(fd)
		{}

		Socket::~Socket()
		{}

		void Socket::close()
		{
			::close(this->_sockfd);
		}

		int Socket::get_fd() const
		{
			return this->_sockfd;
		}
	}
}
