#ifndef WS_CORE_ROUTER_HPP
# define WS_CORE_ROUTER_HPP
# include "../conf/Config.hpp"
# include "../http/Req.hpp"
# include "../http/Res.hpp"
# include "../http/CI_Less.hpp"
# include <dirent.h>

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

			const std::vector<struct dirent>	_getDirList(DIR* dirp) const;
			shared::Buffer			_getFile(const std::string& path) const;
			const conf::Server*		_getServ(http::Req::header_m& header, const conf::host_port& host) const;
			const conf::Server*		_getServerName(const std::string& host, const serv_lst& servLst) const;
			const conf::Location*	_getLocation(const std::string& uri, const conf::Server& serv) const;
			std::string				_getLocalPath(const std::string& uri, const conf::ServConfig& serv) const;
			bool					_checkAcceptedMethod(const conf::Location* loc, e_method method) const;
			bool					_checkMaxBodySize(const conf::ServConfig& serv, size_t bodySize) const;
			void					_checkReq(const conf::ServConfig& servConf, http::Res& response, const http::Req& request) const;
			shared::Buffer			_getBody(const std::string& path, const std::string& uri) const;
			void					_setError(http::Res& response, const conf::ServConfig& serv, const char* str, uint16_t code) const; 
			

			bool					_writeFile(const std::string& path, const shared::Buffer& buff) const;
			const std::string*		_findErrorPage(const conf::ServConfig& serv, const uint16_t code) const;
			shared::Buffer			_getAutoIndexPage(DIR* dirp ,const std::string& uri) const;
		};
	}
}

#endif
