/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:40:33 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/21 18:09:42 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace ws
{
	namespace core
	{
		Router::Router(conf::Config& conf) : _config(conf) {}

		shared::Buffer	_getFile(const std::string& path)
		{
			char				_buff[1024];
			std::ifstream		file(path);
			shared::Buffer		buff;

			if (file)
			{
				while (file.read(_buff, 1024) && !file.eof())
					buff.join(ws::shared::Buffer(_buff, 1024));
				file.close();
			}
			return buff;
		}

		http::Res
		Router::process(http::Req& request, const conf::host_port& host)
		{
			http::Res			response;
			std::string			path;
			shared::Buffer		fileBuff;
			const conf::Server*	serv;
			
			serv = this->_getServ(request.header(), host);
			if (!serv)
			{
				response.setStatus(STATUS444);
				return response;
			}
			path = _getLocalPath(request.path(), *serv);
			if (path.empty()
				|| !(fileBuff = _getFile(path)).size())
				response.setStatus(STATUS404);
			if (request.method() == POST)
				response.setStatus(STATUS201);
			return response;
		}

		const conf::Server*
		Router::_getServ(http::Req::header_m& header, const conf::host_port& host) const
		{
			conf::server_map::iterator it = this->_config.servers.find(host);

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

		std::string
		Router::_getLocalPath(const std::string& uri, const conf::Server& serv) const
		{
			const conf::Location*	loc;
			std::string				root;
			std::string				path;
			struct stat				info;

			loc = this->_getLocation(uri, serv);
			root = !loc || loc->root.empty() ? serv.root : loc->root;
			path = root + uri;
			if (stat(path.c_str(), &info) != 0)
				return std::string();
			if(!S_ISDIR(info.st_mode))
			{
				if (path.back() != '/')
					path += "/";
				path += !loc || loc->index.empty() ? serv.index : loc->index;
			}
			return path;
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
	}
}
