#include <iostream>
#include <string>

#include "shared/Buffer.hpp"
#include "net/Connection.hpp"
#include "net/Server.hpp"

using namespace ws;

int main(void)
{
	net::Server serv;
	net::Connection con;


	serv.listen(9000);
	
	for (;;)
	{
		con = serv.accept();
		std::cout << "Received: " << con.recv(1024).to_string() << std::endl;
		con.send(shared::Buffer(std::string("Goodbye!")));
		con.close();
	}
}
