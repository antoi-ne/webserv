#include "Socket.hpp"

namespace ws
{
	namespace net
	{

		const char * Socket::SocketClosedException::what() const throw()
		{
			return "Socket has been closed and cannot be used.";
		}

		Socket::Socket(int fd)
			: _fd(fd), _open(true)
		{}

		Socket::~Socket()
		{
			this->close();
		}

		std::string Socket::recv(size_t size)
		{
			char *buffer;
			ssize_t ret;
			std::string str;

			if (!this->_open)
				throw SocketClosedException();

			buffer = new char[size + 1]();

			ret = ::recv(this->_fd, buffer, size + 1, 0);

			buffer[ret] = '\0';

			str = std::string(buffer);

			delete [] buffer;

			return str;
		}

		void Socket::send(std::string str)
		{
			ssize_t ret;

			if (!this->_open)
				throw SocketClosedException();

			ret = ::send(this->_fd, str.c_str(), str.length(), 0);
		}

		void Socket::close()
		{
			if (this->_open)
			{
				::close(this->_fd);
				this->_open = false;
			}
		}
	}
}