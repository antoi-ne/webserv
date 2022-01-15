#include <iostream>
#include <string>

#include "shared/Buffer.hpp"
#include "net/Connection.hpp"
#include "net/Server.hpp"
#include "net/Pool.hpp"

using namespace ws;

int main(void)
{
	net::Pool pool;

	pool.add_server(net::Server(9000));
	pool.add_server(net::Server(9001));
	pool.add_server(net::Server(9002));
	pool.add_server(net::Server(9003));
	pool.add_server(net::Server(9004));

	pool.probe();

}
