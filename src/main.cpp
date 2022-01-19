#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

#include "shared/Buffer.hpp"

#include "net/Connection.hpp"
#include "net/Server.hpp"
#include "net/Ctx.hpp"
#include "net/Pool.hpp"
#include "http/Req.hpp"
#include "http/Res.hpp"

using namespace ws;


int main(void)
{
	net::Server srv1(9000, 10);
	net::Server srv2(9001, 10);

	std::list<net::Server> srvs;
	std::list<net::Ctx> ready;
	std::list<net::Ctx>::iterator it;

	srv1.listen();
	srv2.listen();

	srvs.push_back(srv1);
	srvs.push_back(srv2);

	net::Pool pool(srvs);

	for (;;)
	{
		ready = pool.probe();

		for (it = ready.begin(); it != ready.end(); it++)
		{
			std::cout << "connection on port " << it->srv.get_port() << " from " << it->con.get_address() << " is ready for "<< (it->rread?"READ ":" ") << (it->rwrite?"WRITE":"") << std::endl;
			if (it->rread)
			{
				http::Req	request(it->con.recv(2048));
				if (it->rwrite)
				{
					http::Res	response;

					response.sendRes(it->con);
					std::cout << "response sent!" << std::endl;
				}
				pool.close_con(it->con);
				return 0;
			}
		}
	}
} 
