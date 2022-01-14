#include <iostream>
#include <string>


int main(void)
{
	// net::Server server;
	// std::string const PORT = "9000";

	// server.listen(PORT);
	// utils::Log::info("webserv running on port: " + PORT);

	
	// for(;;)
	// {
	// 	net::Socket sock = server.accept();
	// 	sock.send("Hello World!\n");
	// 	std::cout << "received: " << sock.recv(100) << std::endl;
	// 	sock.close();
	// }

	std::string hello("hello");

	std::cout << hello.length() << std::endl;
		std::cout << hello.size() << std::endl;
}
