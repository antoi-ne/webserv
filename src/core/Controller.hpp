#ifndef WS_CORE_CONTROLLER_HPP
# define WS_CORE_CONTROLLER_HPP

# include <vector>
# include <list>
# include <map>

# include "shared/Log.hpp"
# include "net/Pool.hpp"
# include "conf/Config.hpp"

namespace ws
{
	namespace core
	{
		class Controller
		{
		public:
			Controller(conf::Config config = conf::Config());
			~Controller();

			void start();

		private:
			conf::Config _config;
			std::list<net::Server> _srv;
			net::Pool _pool;
			std::map<net::Connection, shared::Buffer> _req_cache;
			std::map<net::Connection, shared::Buffer> _res_cache;

			void _loop();
		};
	}
}

#endif
