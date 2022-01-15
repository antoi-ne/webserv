#include <iostream>
#include <string>

#include "shared/Buffer.hpp"
#include "net/Connection.hpp"
#include "net/Server.hpp"

using namespace ws;

int main(void)
{
	net::Server serv(8900);
	net::Connection con;

	serv.listen(10);
	
	for (;;)
	{
		con = serv.accept();
		std::cout << "Received: " << con.recv(100).to_string();
		con.send(shared::Buffer("Bye!\n"));
		con.close();
	}
}
