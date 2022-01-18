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
				rbytes = ::send(this->get_fd(), buff.get_ptr() + bytes, buff.size(), 0);
				if (rbytes <= 0)
					break;
				bytes -= rbytes;
			}
		}

		shared::Buffer Connection::recv(size_t size)
		{
			char buffer[2048 + 1];
			shared::Buffer buff(size);
			ssize_t rbytes;

			rbytes = ::recv(this->get_fd(), buff.get_ptr(), size, 0);
			if (rbytes < 0)
				return buff;
			if (rbytes == 0)
				shared::Log::error("net::Connection: trying to recv but connection is closed");

			buff = shared::Buffer(buffer, 2048);

			return buff;
		}

		std::string Connection::get_address()
		{
			struct sockaddr_in addr;
			socklen_t addr_len = sizeof(addr);
			char pres[INET6_ADDRSTRLEN];
			int ret;

			ret = getpeername(this->get_fd(), (struct sockaddr *) &addr, &addr_len);
			if (ret < 0)
				return "";

			inet_ntop(addr.sin_family, &addr, pres, INET6_ADDRSTRLEN);

			return pres;
		}

	}
}