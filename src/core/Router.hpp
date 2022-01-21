#ifndef WS_CORE_ROUTER_HPP
# define WS_CORE_ROUTER_HPP
# include "../conf/Config.hpp"
# include "../http/Req.hpp"
# include "../http/Res.hpp"
# include "../http/CI_Less.hpp"

namespace ws
{
	namespace core
	{
		class Router
		{
		public:
			typedef std::vector<conf::Server>	serv_lst;

			Router(conf::Config& _config);

			http::Res	process(http::Req& request, const conf::host_port& host);

		private:
			conf::Config&	_config;

			const conf::Server*	_getServ(http::Req& request, const conf::host_port& host) const;
			const conf::Server* _getServerName(const std::string& host, const serv_lst& servLst) const;
		};
	}
}

#endif
