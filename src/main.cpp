#include <iostream>

#include "./net/Server.hpp"
#include "./conf/Config.hpp"
#include "./conf/Parser.hpp"

using namespace ws;

// int main(void)
// {
// 	conf::Parser parser("./config/example.wsconf");
// }

int main(void)
{
	net::Server server;

	server.listen("9000");
	
	for(;;)
	{
		net::Socket sock = server.accept();
		sock.send("Hello World!\n");
		std::cout << "received: " << sock.recv(100) << std::endl;
		sock.close();
	}
}
