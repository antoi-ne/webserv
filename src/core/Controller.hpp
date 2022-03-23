#ifndef WS_CORE_CONTROLLER_HPP
# define WS_CORE_CONTROLLER_HPP

# include <vector>
# include <list>
# include <map>
# include <utility>
# include <ctime>

# include "shared/Log.hpp"
# include "shared/Buffer.hpp"
# include "shared/Option.hpp"
# include "conf/Config.hpp"
# include "net/Pool.hpp"
# include "http/Req.hpp"
# include "http/Parser/ReqParser.hpp"
# include "http/MsgUpdate.hpp"
# include "http/Res.hpp"
# include "core/Router.hpp"
# include "cgi/Launcher.hpp"

namespace ws
{
	namespace core
	{
		class Controller
		{
		public:
			typedef http::MsgUpdate<http::Req, http::ReqParser>	Req;

			Controller(const conf::Config& config);
			~Controller();

			void start();

		private:
			conf::Config _config;
			std::list<net::Server> _srv;
			net::Pool _pool;
			std::map<net::Connection, Req> _req_cache;
			std::map<net::Connection, std::pair<shared::Buffer,bool> > _res_cache;
			core::Router _router;
			void		_checkTimeout(std::map<net::Connection, Req>::iterator rqit);

			void _loop();
		};
	}
}

#endif
