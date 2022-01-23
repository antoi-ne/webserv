/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:40:33 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/23 16:27:14 by vneirinc         ###   ########.fr       */
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
			http::Res			response;
			const conf::Server*	serv;
			
			if ((serv = this->_getServ(request.header(), host)))
				this->_processServ(response, request, *serv);
			else
				response.setStatus(STATUS444);
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
					buff.join(ws::shared::Buffer(_buff, len));
				}
				file.close();
			}
			return buff;
		}

		void
		Router::_processServ(
			http::Res& response,
			const http::Req& request,
			const conf::Server& serv) const
		{
			const conf::Location*	loc;
			std::string				path;
			shared::Buffer			fileBuff;

			loc = this->_getLocation(request.path(), serv);
			path = _getLocalPath(request.path(), serv, loc);
			if (path.empty() || !(fileBuff = _getFile(path)).size())
				response.setStatus(STATUS404);
			if (request.method() == POST)
				response.setStatus(STATUS201);
			response.body().join(fileBuff);
			if (loc) (void)true; // check accepted method
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
			const conf::Server& serv,
			const conf::Location* loc) const
		{
			std::string				path;
			struct stat				info;

			path = !loc || loc->root.empty() ? serv.root : loc->root;
			path += uri;
			if (path.back() == '/') // Maybe if '/' force dir?
				path.pop_back();
			if (stat(path.c_str(), &info) != 0)
				return std::string();
			if (S_ISDIR(info.st_mode))
			{
				path += "/";
				path += !loc || loc->index.empty() ? serv.index : loc->index;
			}
			return path;
		}
	}
}
