#ifndef WS_CORE_CONTROLLER_HPP
# define WS_CORE_CONTROLLER_HPP

# include <vector>
# include <list>
# include <map>
# include <utility>

# include "shared/Log.hpp"
# include "conf/Config.hpp"
# include "net/Pool.hpp"
# include "http/Req.hpp"
# include "http/Res.hpp"
# include "core/Router.hpp"

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
			std::map<net::Connection, http::Req> _req_cache;
			std::map< net::Connection, std::vector<http::Res> > _res_cache;
			core::Router _router;

			void _loop();
		};
	}
}

#endif
