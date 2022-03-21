#include "conf/Parser.hpp"
#include "core/Controller.hpp"

int main(int ac, char* av[])
{
	if (ac > 2)
		return 1;
	ws::conf::Parser	parser(av[1] ? av[1] : "./config/ws1.conf");

	parser.print_config(parser.config.servers);

	ws::core::Controller control = ws::core::Controller(parser.config);

	control.start();
	return 0;
}
