#ifndef WS_CORE_ROUTER_HPP
# define WS_CORE_ROUTER_HPP
# include "../conf/Config.hpp"
# include "../http/Req.hpp"
# include "../http/Res.hpp"
# include "../http/CI_Less.hpp"
# include "MIME.h"
# include <dirent.h>

namespace ws
{
	namespace core
	{
		class Router
		{
		public:
			typedef std::vector<conf::Server>	serv_lst;
			// extension -> type;
			typedef std::vector<std::pair<std::vector<std::string>, std::string> > mime_vec;

			Router(const conf::Config& _config);

			http::Res	process(const http::Req& request, const conf::host_port& host) const;

		private:
			const conf::Config&	_config;
			mime_vec			_mime;

			void								_process(const http::Req& request, http::Res& response,const conf::ServConfig& mainConf, const conf::host_port& host) const;
			std::pair<const char *, uint16_t>	_processServ(const http::Req& request, http::Res& response, const conf::ServConfig& mainConf, const conf::host_port& host) const;

			std::pair<const char *, uint16_t>	_upload( const http::Req& request, const conf::ServConfig& mainConf) const;

			std::string				_getExt(const std::string& path) const;
			void					_getMIME(http::Res& res, const std::string& ext) const;
			void					_initMIME(const std::string mime_type, const std::string mime_ext);
			const std::vector<struct dirent>	_getDirList(DIR* dirp) const;
			bool					_getFile(shared::Buffer& body, const std::string& path) const;
			const conf::Server*		_getServ(const std::string& headerHost, const conf::host_port& host) const;
			const conf::Server*		_getServerName(const std::string& host, const serv_lst& servLst) const;
			const conf::Location*	_getLocation(const std::string& uri, const conf::Server& serv) const;
			std::string				_getLocalPath(const e_method method, const std::string& uri, const conf::ServConfig& serv) const;
			bool					_checkMaxBodySize(const conf::ServConfig& serv, size_t bodySize) const;
			void					_checkReq(const conf::ServConfig& servConf, http::Res& response, const http::Req& request) const;
			bool					_getBody(shared::Buffer& body, const std::string& path, const std::string& uri) const;
			void					_setError(http::Res& response, const conf::ServConfig& serv, const char* str, uint16_t code) const; 
			std::pair<const char *, uint16_t>	_renderPage(const http::Req& request, http::Res& response, const conf::ServConfig& mainConf, const conf::host_port& host) const;
			bool					_delete(const std::string& path) const;
			bool					_deleteDir(const std::string& path, DIR* dirp) const;

			bool					_writeFile(const std::string& path, const shared::Buffer& buff) const;
			const std::string*		_findErrorPage(const conf::ServConfig& serv, const uint16_t code) const;
			shared::Buffer			_getAutoIndexPage(DIR* dirp ,const std::string& uri) const;

			std::pair<std::string, ws::shared::Buffer>	_parseFormData(const std::string& boundary, const ws::shared::Buffer& body) const;
			std::string 			_getBoundary(std::string headerField) const;
		};
	}
}

#endif
