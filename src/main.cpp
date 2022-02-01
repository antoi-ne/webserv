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
#include "http/Req.hpp"
#include "http/Res.hpp"

#include "core/Controller.hpp"

using namespace ws;


int main(void)
{
	conf::Config cfg;
	conf::Server srv;
	conf::Location loc;

	srv.root = "./assets/ws1";
	srv.index = "index.html";
	srv.server_names.push_back("localhost");
	srv.locations.clear();
	srv.accepted_methods.push_back(GET);

	cfg.servers[std::make_pair("0.0.0.0", 80U)].push_back(srv);

	core::Controller control = core::Controller(cfg);

	control.start();
}
