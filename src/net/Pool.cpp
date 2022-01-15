#include "Pool.hpp"

namespace ws
{
	namespace net
	{
		Pool::Pool()
		{}

		Pool::~Pool()
		{}

		void Pool::add_server(Server srv)
		{
			this->_srv.push_back(srv);
		}

		void Pool::add_connection(Connection con)
		{
			this->_con.push_back(con);
		}

		void Pool::probe()
		{
			
		}
	}
}