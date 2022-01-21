/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:40:33 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/20 19:02:06 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"

namespace ws
{
	namespace core
	{
		Router::Router(conf::Config& conf) : _config(conf) {}

		http::Res	Router::process(http::Req& request, const std::string& host, u_int16_t port)
		{
			http::Res	response;
			conf::Server	winner;
			Server_lst	vServers;
			Server_lst	servers = this->_config.servers;
			std::string	vHost = request.header("Host");

			for (Server_lst::iterator it = servers.begin(); it != servers.end(); ++it)
				if (it->host == host && it->port == port)
					vServers.push_back(*it);
			
			for (Server_lst::iterator it = vServers.begin(); it != vServers.end(); ++it)
				for (std::vector<std::string>::iterator iter = it->server_names.begin(); iter != it->server_names.end(); ++iter)
				{
					if (vHost == *iter)
					{
						winner = *it;
						break ;
					}
				}
			return response;
		}
	}
}
