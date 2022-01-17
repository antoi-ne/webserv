#include "Pool.hpp"

namespace ws
{
	namespace net
	{
		Pool::Pool(std::list<Server> srv)
		{
			std::list<Server>::iterator it;

			FD_ZERO(&this->_set);
			this->_fdmax = 0;
			this->_srv = srv;

			for (it = srv.begin(); it != srv.end(); it++)
			{
				FD_SET(it->get_fd(), &this->_set);
				if (it->get_fd() > this->_fdmax)
					this->_fdmax = it->get_fd();
			}
		}

		Pool::~Pool()
		{}

		std::list< std::pair<Connection, Server> > Pool::probe()
		{
			fd_set reader_set;
			net::Connection con;
			std::list< std::pair<Connection, Server> >::iterator cit;
			std::list<Server>::iterator sit;
			std::list< std::pair<Connection, Server> > ready;

			FD_ZERO(&reader_set);

			reader_set = this->_set;

			if (select(this->_fdmax + 1, &reader_set, NULL, NULL, NULL) < 0)
			{
				shared::Log::error("net::Pool syscall select failed");
				return ready;
			}

			for (cit = this->_con.begin(); cit != this->_con.end(); cit++)
			{
				if (FD_ISSET(cit->first.get_fd(), &reader_set))
				{
					shared::Log::info("net::Pool connection ready to read");
					ready.push_back(*cit);
				}
			}

			// called AFTER the connections loop to prevent new cons to be checked
			for (sit = this->_srv.begin(); sit != this->_srv.end(); sit++)
			{
				if (FD_ISSET(sit->get_fd(), &reader_set))
				{
					if ((con = sit->accept()).get_fd() < 0)
					{
						shared::Log::error("net::Pool accept failed");
						continue;
					}
					shared::Log::info("net::Pool new connection");
					this->_con.push_back(std::make_pair(con, *sit));
					FD_SET(con.get_fd(), &this->_set);
					if (con.get_fd() > this->_fdmax)
						this->_fdmax = con.get_fd();
				}
			}

			return ready;
		}
	}
}