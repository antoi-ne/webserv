/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:40:33 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/21 13:00:56 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"

namespace ws
{
	namespace core
	{
		Router::Router(conf::Config& conf) : _config(conf) {}

		bool	_findPath(void)
		{return true;}

		http::Res	Router::process(http::Req& request, const conf::host_port& host)
		{
			http::Res			response;
			const conf::Server*	serv = this->_getServ(request, host);

			if (!serv)
			{
				response.setStatus(STATUS444);
				return response;
			}

			std::string	path = this->_getRealPath(request.path(), *serv);

			if (path.empty() || !_findPath())
				response.setStatus(STATUS404);

			return response;
		}

		const conf::Server*	Router::_getServ(http::Req& request, const conf::host_port& host) const
		{
			conf::server_map::iterator it = this->_config.servers.find(host);

			if (it == this->_config.servers.end() || it->second.empty())
				return nullptr;
			return _getServerName(request.header("host"), it->second);
		}

		const conf::Server* Router::_getServerName(const std::string& host, const serv_lst& servLst) const
		{
			if (servLst.size() > 1)
			{
				CI_Less	ci;

				for (serv_lst::const_iterator it = servLst.begin(); it != servLst.end(); ++it)
					for (std::vector<std::string>::const_iterator iter = it->server_names.begin(); iter != it->server_names.end(); ++iter)
						if (!ci(*iter, host) && !ci(host, *iter))
							return it.base();
			}
			return servLst.begin().base();
		}

		const std::string&	Router::_getRealPath(const std::string& reqPath, const conf::Server& serv) const
		{
			conf::location_map::const_iterator	it = serv.locations.find(reqPath);

			if (it == serv.locations.end())
				return std::string();
			return it->first;
		}
	}
}
