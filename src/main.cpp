#include <iostream>
#include <string>
#include <list>

#include "shared/Buffer.hpp"
#include "net/Connection.hpp"
#include "net/Server.hpp"
#include "net/Pool.hpp"

using namespace ws;

int main(void)
{
	std::list<net::Server> srvs;
	std::list<net::Connection> ready;
	std::list<net::Connection>::iterator it;

	srvs.push_back(net::Server(9000));
	srvs.push_back(net::Server(9001));

	net::Pool pool(srvs);

	for (;;)
	{
		ready = pool.probe();

		for (it = ready.begin(); it != ready.end(); it++)
		{
			std::cout << "received: " << it->recv(1024);
		}
	}
}
