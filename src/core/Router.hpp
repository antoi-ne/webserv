#ifndef WS_CORE_ROUTER_HPP
# define WS_CORE_ROUTER_HPP
# include "../conf/Config.hpp"
# include "../http/Req.hpp"
# include "../http/Res.hpp"

namespace ws
{
	namespace core
	{
		class Router
		{
		public:
			typedef std::vector<conf::Server> Server_lst; 

			Router(conf::Config& _config);

			http::Res	process(http::Req& request, const std::string& host, u_int16_t port);


		private:
			conf::Config&	_config;

			Server_lst		_portFilter(Server_lst& servers);
		};
	}
}

#endif
