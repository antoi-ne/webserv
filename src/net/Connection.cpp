#include "Connection.hpp"
#include <iostream>

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
	}
}