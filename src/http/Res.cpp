/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Res.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:15:36 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/22 14:33:54 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Res.hpp"
#include <ctime>
#include <cmath>

namespace http
{

	Res::Res(void)
	 : _statusMsg(STATUS200), _contentType("text/html"), _body()
	{}

	void	Res::sendRes(ws::net::Connection& conn) const
	{
		time_t	t = time(0);
		tm		*ltm = gmtime(&t);
		char	date_buff[256];

		size_t s = strftime(date_buff, 255, DATE_FORMAT, ltm);
		date_buff[s] = '\0';

		size_t	buffSize = 256 + s + this->_statusMsg.size() + this->_contentType.size() + this->_body.size();
		char	*_buff = new char[buffSize];

		size_t size = sprintf(_buff,
			HTTPVER" %s\r\n"
			"Server: " SERVER"\r\n"
			"Date: %s\r\n"
			"Content-Type: %s\r\n"
			"Content-Length: %lu\r\n"
			"Connection: %s\r\n"
			"Accept-Ranges: %s\r\n\r\n",
			this->_statusMsg.c_str(),
			date_buff,
			this->_contentType.c_str(),
			this->_body.size(),
			"keep-alive",
			"bytes"
		);
		conn.send(ws::shared::Buffer(_buff, size));
		if (this->_body.size())
			conn.send(this->_body);
	}

	void	Res::setStatus(const std::string& statusMsg)
	{ this->_statusMsg = statusMsg; }

	void	Res::setContentType(const std::string& contentType)
	{ this->_contentType = contentType; }

	ws::shared::Buffer&	Res::body(void)
	{ return this->_body; }
}