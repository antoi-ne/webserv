#include <iostream>

#include "./net/Server.hpp"

using namespace ws;

int main(void)
{
	net::Server server;

	server.listen("9000");

	for(;;)
	{
		net::Socket sock = server.accept();
		sock.send("Hello World!\n");
		std::string s = sock.recv(50);
		std::cout << "received: " << s << std::endl;
		sock.close();
	}
}
