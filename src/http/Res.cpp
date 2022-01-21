/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Res.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:15:36 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/21 19:55:33 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Res.hpp"
#include <ctime>

namespace http
{

	Res::Res(void)
	 : _statusMsg(STATUS200), _contentType("text/html"), _contentLength(0)
	{}

	void Res::_getDate(std::string& buff) const
	{
		time_t	t = time(0);
		tm		*ltm = gmtime(&t);
		char	date_buff[256];

		size_t s = strftime(date_buff, 255, DATE_FORMAT, ltm);
		date_buff[s] = '\0';
		buff.append(date_buff);
	}

	void	Res::_defaultRes(std::string& buff) const
	{
		buff.append(this->_statusMsg + "\r\n");
		buff.append("Server: webserv/0.1.0\r\n");
		this->_getDate(buff);
	}

	void	Res::sendRes(ws::net::Connection& conn) const
	{
		std::string	buff(HTTPVER);

		this->_defaultRes(buff);
		buff.append("Content-Type: " + this->_contentType + "\r\n");
		buff.append("Content-Length: " + std::to_string(this->_contentLength) + "\r\n");
		buff.append("Connection: keep-alive\r\n");
		buff.append("Accept-Ranges: bytes\r\n\r\n");
		conn.send(buff);
	}

	void	Res::setStatus(const std::string& statusMsg)
	{ this->_statusMsg = statusMsg; }
}