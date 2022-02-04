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

	core::Controller control = core::Controller(parser.config);
	control.start();
}
