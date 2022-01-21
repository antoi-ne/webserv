/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Res.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:15:36 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/20 16:45:39 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Res.hpp"
#include <ctime>

namespace http
{

	Res::Res(void)
	 : _status(200), _statusMsg("OK"), _contentType("text/html"), _contentLength(0)
	{}

	void Res::_getDate(std::string& buff) const
	{
		time_t	t = time(NULL);
		tm		*ltm = gmtime(&t);
		char	date_buff[256];

		strftime(date_buff, sizeof(date_buff), DATE_FORMAT, ltm);
		buff += date_buff;
	}

	void	Res::_defaultRes(std::string& buff) const
	{
		buff += " " + std::to_string(this->_status) + " " + this->_statusMsg + "\r\n";
		buff += "Server: webserv/0.1.0\r\n";
		this->_getDate(buff);
	}

	void	Res::sendRes(ws::net::Connection& conn) const
	{
		std::string	buff(HTTPVER);

		this->_defaultRes(buff);
		buff += "Content-Type: " + this->_contentType + "\r\n";
		buff += "Content-Length: " + std::to_string(this->_contentLength) + "\r\n";
		buff += "Connection: keep-alive\r\n";
		buff += "Accept-Ranges: bytes\r\n";
		conn.send(buff);
	}
}