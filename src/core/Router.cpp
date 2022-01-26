/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:40:33 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/26 09:16:29 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

namespace ws
{
	namespace core
	{
		Router::Router(const conf::Config& conf) : _config(conf) {}

		http::Res
		Router::process(http::Req& request, const conf::host_port& host) const
		{
			http::Res				response;
			const conf::Server*		serv;
			
			if ((serv = this->_getServ(request.header(), host)))
			{
				this->_processServ(response, request, *serv);
			}
			else
				this->_setError(response, *serv, STATUS444, 444);
			return response;
		}

		const conf::Server*
		Router::_getServ(http::Req::header_m& header, const conf::host_port& host) const
		{
			conf::server_map::const_iterator it = this->_config.servers.find(host);

			if (it == this->_config.servers.end() || it->second.empty())
				return nullptr;
			return _getServerName(header["host"], it->second);
		}

		const conf::Server*
		Router::_getServerName(const std::string& host, const serv_lst& servLst) const
		{
			if (servLst.size() > 1 && !host.empty())
			{
				CI_Less	ci;

				for (serv_lst::const_iterator it = servLst.begin(); it != servLst.end(); ++it)
					for (std::vector<std::string>::const_iterator iter = it->server_names.begin(); iter != it->server_names.end(); ++iter)
						if (!ci(*iter, host) && !ci(host, *iter))
							return it.base();
			}
			return servLst.begin().base();
		}

		shared::Buffer	_getFile(const std::string& path)
		{
			std::ifstream		file(path);
			shared::Buffer		buff;

			if (file)
			{
				char	_buff[2048];
				size_t	len;

				while (!file.eof())
				{
					file.read(_buff, 2048);
					len = file.gcount();
					buff.join(shared::Buffer(_buff, len));
				}
				file.close();
			}
			return buff;
		}

		bool	Router::_checkAcceptedMethod(const conf::Location* loc, e_method method) const
		{
			if (loc)
			{
				if (std::find(loc->accepted_methods.begin(), loc->accepted_methods.end(), method) != loc->accepted_methods.end())
					return true;
			}
			else
				return method == GET;
			return false;
		}

		bool	Router::_checkMaxBodySize(const conf::ServConfig& serv, size_t bodySize) const
		{
			if (serv.max_body_size != -1)
				return bodySize <= static_cast<size_t>(serv.max_body_size);
			return true;
		}

		void
		Router::_setError(
			http::Res& response,
			const conf::ServConfig& serv,
			const char* str,
			const uint16_t code) const
		{
			const std::string*	errorPage;

			response.setStatus(str);
			errorPage = this->_findErrorPage(serv, code);
			if (errorPage != NULL)
				response.body().join(*errorPage);
		}

		const std::string*
		Router::_findErrorPage(const conf::ServConfig& serv, const uint16_t code) const
		{
			conf::ErrorPages::const_iterator it = serv.error_pages.find(code);

			if (it == serv.error_pages.end())
				return NULL;
			return it->second;
		}

		void
		Router::_processServ(
			http::Res& response,
			const http::Req& request,
			const conf::Server& serv) const
		{
			const conf::Location*	loc;
			std::string				path;
			shared::Buffer			body;
			conf::ServConfig		mainConf;

			if ((loc = this->_getLocation(request.path(), serv)))
				mainConf = *loc;
			else
				mainConf = serv;
			path = _getLocalPath(request.path(), mainConf);
			if (path.empty())
			{
				return this->_setError(response, mainConf, STATUS404, 404);
			}
			if (path.back() != '/')
			{
 				if (!(body = _getFile(path)).size())
					return this->_setError(response, mainConf, STATUS403, 403);
			}
			else
			{
				// Make autoindex
			}
			if (!this->_checkAcceptedMethod(loc, request.method()))
			{
				return this->_setError(response, mainConf, STATUS405, 405);
			}
			if (!this->_checkMaxBodySize(mainConf, request.body().size()))
			{
				return this->_setError(response, mainConf, STATUS413, 413);
			}
			if (request.method() == POST)
				response.setStatus(STATUS201);
			response.body().join(body);
		}

		const conf::Location*
		Router::_getLocation(const std::string& uri, const conf::Server& serv) const
		{
			std::string	path = uri;
			conf::location_map::const_iterator	it = serv.locations.find(path);

			while (it == serv.locations.end() && !path.empty())
			{
				size_t	lastDir = path.find_last_of('/');
				path = path.substr(0, path.size() - lastDir - 1);
				it = serv.locations.find(path);
			}
			if (path.empty())
				return NULL;
			return std::addressof(it->second);
		}

		std::string
		Router::_getLocalPath(
			const std::string& uri,
			const conf::ServConfig& serv) const
		{
			std::string	path;
			struct stat	info;

			path = serv.root;
			path += uri;
			if (path.back() == '/')
				path.pop_back();
			if (stat(path.c_str(), &info) != 0)
				return std::string();
			if (S_ISDIR(info.st_mode))
			{
				path.push_back('/');
				if (!serv.autoindex)
					path.append(serv.index);
			}
			return path;
		}
	}
}
