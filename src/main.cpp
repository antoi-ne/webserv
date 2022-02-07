#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

#include "shared/Buffer.hpp"

# include "conf/Parser.hpp"

#include "net/Connection.hpp"
#include "net/Server.hpp"
#include "net/Ctx.hpp"
#include "net/Pool.hpp"

#include "core/Controller.hpp"

using namespace ws;


int main(int ac, char* av[])
{
	if (ac > 2)
		return 1; (void)av;
	conf::Parser	parser("./config/ws1.conf");
	conf::Server	serv;
	conf::Location	loc;


	// TEST serv1
	serv.accepted_methods.push_back(GET);
	serv.server_names.push_back("serv1");
	serv.server_names.push_back("1serv");
	serv.root = "./assets/ws1";
	serv.index = "empty.html";
	serv.max_body_size = -1;

	loc.route = "/serv1";
	loc.accepted_methods.push_back(GET);
	loc.root = serv.root;
	loc.index = serv.index;
	loc.max_body_size = -1;

	serv.locations.push_back(loc);
	parser.config.servers[std::make_pair("127.0.0.1", 8080)].push_back(serv);

	serv = conf::Server();
	loc = conf::Location();
	// TEST serv2
	serv.accepted_methods.push_back(GET);
	serv.server_names.push_back("serv2");
	serv.server_names.push_back("serv2:8080");
	serv.root = "./assets/ws1";
	serv.index = "empty.html";
	serv.max_body_size = -1;

	loc.route = "/serv2";
	loc.accepted_methods.push_back(GET);
	loc.root = serv.root;
	loc.index = serv.index;
	loc.max_body_size = -1;

	conf::host_port	host = std::make_pair("127.0.0.1", 8080);
	serv.locations.push_back(loc);
	parser.config.servers[host].push_back(serv);

	for (conf::location_v::iterator	it = parser.config.servers[host].front().locations.begin(); it != parser.config.servers[host].front().locations.end(); ++it)
		if (it->route == "/upload" || it->route == "/post")
			it->max_body_size = 1024;
	parser.print_config(parser.config.servers);
	core::Controller control = core::Controller(parser.config);
	control.start();
}
