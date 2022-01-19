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

		std::list<Ctx> Pool::probe()
		{
			fd_set read_set;
			fd_set write_set;
			net::Connection con;
			std::list<std::pair<Connection, Server> >::iterator cit;
			std::list<Server>::iterator sit;
			Ctx current;
			std::list<Ctx> ready;

			FD_ZERO(&read_set);
			FD_ZERO(&write_set);

			read_set = this->_set;
			write_set = this->_set;

			if (select(this->_fdmax + 1, &read_set, &write_set, NULL, NULL) < 0)
			{
				shared::Log::error("net::Pool syscall select failed");
				return ready;
			}

			for (cit = this->_con.begin(); cit != this->_con.end(); cit++)
			{
				current = Ctx(cit->first, cit->second);

				if (FD_ISSET(cit->first.get_fd(), &read_set))
					current.rread = true;

				if (FD_ISSET(cit->first.get_fd(), &write_set))
					current.rwrite = true;

				if (current.rread || current.rwrite)
					ready.push_back(current);
			}

			// called AFTER the connections loop to prevent new cons to be checked
			for (sit = this->_srv.begin(); sit != this->_srv.end(); sit++)
			{
				if (FD_ISSET(sit->get_fd(), &read_set))
				{
					for (;;)
					{
						if ((con = sit->accept()).get_fd() <= 0)
							break;
						shared::Log::info("net::Pool new connection");
						this->_con.push_back(std::make_pair(con, *sit));
						FD_SET(con.get_fd(), &this->_set);
						if (con.get_fd() > this->_fdmax)
							this->_fdmax = con.get_fd();
					}
				}
			}

			return ready;
		}

		void Pool::close_con(Connection con)
		{
			std::list<std::pair<Connection, Server> >::iterator it;
			int fdmax = 0;

			for (it = this->_con.begin(); it != this->_con.end(); it++)
			{
				if (it->first.get_fd() == con.get_fd())
				{
					if (FD_ISSET(it->first.get_fd(), &this->_set))
						FD_CLR(it->first.get_fd(), &this->_set);
					it->first.close();
					this->_con.erase(it);
					break;
				}
			}

			if (it == this->_con.end())
				shared::Log::fatal("net::Pool: trying to remove connection thas is not repertoried, this should not happen");

			for (it = this->_con.begin(); it != this->_con.end(); it++)
			{
				if (it->first.get_fd() > fdmax)
					fdmax = it->first.get_fd();
			}
		}
	}
}