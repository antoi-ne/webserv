#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

#include "shared/Buffer.hpp"

#include "net/Connection.hpp"
#include "net/Server.hpp"
#include "net/Pool.hpp"
#include "http/Req.hpp"
#include "http/Res.hpp"

using namespace ws;


int main(void)
{
	net::Server srv1(9000);
	net::Server srv2(9001);

	std::list<net::Server> srvs;
	std::list<std::pair<net::Connection, net::Server> > ready;
	std::list<std::pair<net::Connection, net::Server> >::iterator it;

	srv1.listen(10);
	srv2.listen(10);

	srvs.push_back(srv1);
	srvs.push_back(srv2);

	net::Pool pool(srvs);

	for (;;)
	{
		ready = pool.probe();

		for (it = ready.begin(); it != ready.end(); it++)
		{
			http::Res	res;
			std::cout << "received from " << it->first.get_address() << " on port [" << it->second.get_port() << "] ";
			res.sendRes(it->first);
		}
	}
}
