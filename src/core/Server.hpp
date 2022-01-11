#ifndef WS_SERVER_HPP
#define WS_SERVER_HPP

#include <list>

namespace ws
{
	class Server
	{
	public:

		Server();
		~Server();

		void Listen();

	private:

		std::list<int> _ports;
		
	}
}

#endif
