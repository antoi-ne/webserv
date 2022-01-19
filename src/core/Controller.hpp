#ifndef WS_CORE_CONTROLLER_HPP
#define WS_CORE_CONTROLLER_HPP

#include <vector>
#include <map>

#include "shared/Log.hpp"
#include "net/Pool.hpp"

namespace ws
{
	namespace core
	{
		class Controller
		{
		public:
			Controller();
			Controller(void *config);
			~Controller();

		private:
			void *_conf;
			std::vector<net::Server> _srv;
			// net::Pool _pool;
			std::map<net::Connection, shared::Buffer> _req_cache;
			std::map<net::Connection, void *> _res_cache;
		};
	}
}

#endif
