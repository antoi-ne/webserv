#include "Connection.hpp"

namespace ws
{
	namespace net
	{
		Connection::Connection(int fd)
			: Socket(fd)
		{}

		void Connection::send(shared::Buffer buff)
		{
			ssize_t bytes = buff.size();
			ssize_t rbytes = 0;

			while (bytes > 0)
			{
				rbytes = ::send(this->get_fd(), buff.get_ptr() + rbytes, bytes, 0);
				if (rbytes <= 0)
					break;
				bytes -= rbytes;
			}
		}

		shared::Buffer Connection::recv(size_t size)
		{
			char *buffer;
			shared::Buffer buff;
			ssize_t rbytes;

			buffer = new char[size + 1]();
			rbytes = ::recv(this->get_fd(), buffer, size, 0);
			if (rbytes < 0)
			{
				delete [] buffer;
				return buff;
			}

			if (rbytes == 0)
				shared::Log::error("net::Connection: trying to recv but connection is closed");

			buff = shared::Buffer(buffer, rbytes);

			delete [] buffer;
			return buff;
		}

		std::string Connection::get_address()
		{
			struct sockaddr_in addr;
			socklen_t addr_len = sizeof(addr);
			char pres[INET_ADDRSTRLEN];
			int ret;

			ret = getpeername(this->get_fd(), (struct sockaddr *) &addr, &addr_len);
			if (ret < 0)
				return "";

			inet_ntop(addr.sin_family, &addr, pres, INET_ADDRSTRLEN);

			return pres;
		}

		bool Connection::operator<(const Connection& rhs) const
		{
			return this->get_fd() < rhs.get_fd();
		}

		bool Connection::operator==(const Connection& rhs) const
		{
			return this->get_fd() == rhs.get_fd();
		}
	}
}