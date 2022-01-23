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

			Router(const conf::Config& _config);

			http::Res	process(http::Req& request, const conf::host_port& host) const;

		private:
			const conf::Config&	_config;

			void	_processServ(http::Res& response, const http::Req& request, const conf::Server& serv) const;

			const conf::Server*		_getServ(http::Req::header_m& header, const conf::host_port& host) const;
			const conf::Server*		_getServerName(const std::string& host, const serv_lst& servLst) const;
			const conf::Location*	_getLocation(const std::string& uri, const conf::Server& serv) const;
			std::string				_getLocalPath(const std::string& uri, const conf::Server& serv, const conf::Location* loc) const;
		};
	}
}

#endif
