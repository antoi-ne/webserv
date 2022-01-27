#include "Connection.hpp"

namespace ws
{
	namespace net
	{
		Connection::Connection(int fd)
			: Socket(fd)
		{}

		ssize_t Connection::send(shared::Buffer buff)
		{
			ssize_t bytes = buff.size();
			ssize_t rbytes = 0;

			rbytes = ::send(this->get_fd(), buff.get_ptr() + rbytes, bytes, 0);
			return rbytes;
		}

		shared::Option<shared::Buffer> Connection::recv(size_t size)
		{
			char *buffer;
			shared::Buffer buff;
			ssize_t rbytes;

			buffer = new char[size + 1]();
			rbytes = ::recv(this->get_fd(), buffer, size, 0);
			if (rbytes <= 0)
			{
				delete [] buffer;
				return shared::Option<shared::Buffer>();
			}

			buff = shared::Buffer(buffer, rbytes);

			delete [] buffer;
			return shared::Option<shared::Buffer>(buff);
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