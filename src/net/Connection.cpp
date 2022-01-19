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
			::send(this->get_fd(), buff.get_ptr(), buff.size(), 0);
		}

		shared::Buffer Connection::recv(size_t size)
		{
			shared::Buffer buff(size);
			::recv(this->get_fd(), buff.get_ptr(), buff.size(), 0);

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