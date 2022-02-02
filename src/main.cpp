#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>

#include "shared/Buffer.hpp"

# include "conf/Config.hpp"

#include "net/Connection.hpp"
#include "net/Server.hpp"
#include "net/Ctx.hpp"
#include "net/Pool.hpp"

#include "core/Controller.hpp"

using namespace ws;


int main(void)
{
	conf::Config cfg;
	conf::Server srv;
	conf::Location loc;

	// loc.root = "./assets/upload";
	// loc.index = "test.html";
	// loc.route = "/test";
	// loc.accepted_methods.push_back(GET);
	// loc.autoindex = true;
	// loc.upload_path = "./assets/upload";

	// srv.max_body_size = -1;
	// srv.root = "./assets/ws1";
	// srv.accepted_methods.push_back(GET);
	// srv.index = "index.html";
	// srv.server_names.push_back("localhost");

	loc.root = "./assets/ws2";
	loc.route = "/directory";
	loc.accepted_methods.push_back(GET);
	loc.accepted_methods.push_back(POST);
	loc.accepted_methods.push_back(PUT);
	loc.max_body_size = -1;
	loc.cgi_ext = "bla";
	// loc.cgi_script = "/usr/local/bin/python3";
	loc.cgi_script = "/Users/ancoulon/Documents/42/projects/webserv/scripts/cgi_tester";

	srv.max_body_size = -1;
	srv.root = "./assets/ws1";
	srv.accepted_methods.push_back(GET);
	srv.index = "index.html";
	srv.server_names.push_back("localhost");

	srv.locations.push_back(loc);

	cfg.servers[std::make_pair("0.0.0.0", 80U)].push_back(srv);

	core::Controller control = core::Controller(cfg);

	control.start();
}
