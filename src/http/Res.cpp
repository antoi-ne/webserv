/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Res.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:15:36 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/21 19:22:56 by vneirinc         ###   ########.fr       */
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
		char	date_buff[257];

		size_t s = strftime(date_buff, sizeof(date_buff), DATE_FORMAT, ltm);
		date_buff[s] = '\0';
		buff += std::string(date_buff);
	}

	void	Res::_defaultRes(std::string& buff) const
	{
		buff += " " + this->_statusMsg + "\r\n";
		buff += "Server: webserv/0.1.0\r\n";
		this->_getDate(buff);
	}

	void	Res::sendRes(ws::net::Connection& conn) const
	{
		std::string	buff(HTTPVER);

		this->_defaultRes(buff);
		std::cout << "s: " << buff << std::endl;
		buff += "Content-Type: ";
		buff += this->_contentType;
		buff += "\r\n";
		buff += "Content-Length: " + std::to_string(this->_contentLength) + "\r\n";
		buff += "Connection: keep-alive\r\n";
		buff += "Accept-Ranges: bytes\r\n";
		std::cout << "buff = " << this->_contentType << std::endl;
		conn.send(buff);
	}

	void	Res::setStatus(const std::string& statusMsg)
	{ this->_statusMsg = statusMsg; }
}