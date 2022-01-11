#include "./net/Server.hpp"

using namespace ws;

void client_handler(int fd)
{
	send(fd, "Hello World!\n", 13, 0);
}

int main(void)
{
	net::Server serv("9000", 5, &client_handler);

	serv.listen();
}
