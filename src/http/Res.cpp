/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Res.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:15:36 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/01 17:04:02 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Res.hpp"
#include <ctime>
#include <cmath>
#include <iostream>

namespace http
{
	Res::Res(void)
	 : Message(), _statusMsg(STATUS200)
	{
		this->_header.insert(std::make_pair("Connection", "keep-alive"));
		this->_header.insert(std::make_pair("Content-Type", "text/html"));
	}

	std::string	Res::_getTime(void) const
	{
		time_t	t = time(0);
		tm		*ltm = gmtime(&t);
		char	date_buff[256];

		size_t s = strftime(date_buff, 255, DATE_FORMAT, ltm);
		date_buff[s] = '\0';
		return date_buff;
	}

	ws::shared::Buffer Res::get_res()
	{
		std::string	buff;
		std::string		crlf("\r\n"); 

		buff += HTTPVER" ";
		buff += this->_statusMsg + crlf;

		buff += "Server: ";
		buff += SERVER"\r\n";

		buff += "Date: ";
		buff += this->_getTime() + crlf;

		if (this->_contentLength == std::string::npos)
		{
			buff += "Content-Length: ";
			buff += std::to_string(this->_body.size());
			buff += crlf;
		}
		for (header_m::const_iterator it = this->_header.begin(); it != this->_header.end(); ++it)
			buff += (it->first + std::string(": ") + it->second + crlf);
		buff += crlf;
		
		ws::shared::Buffer	_buff(buff);

		if (this->_body.size())
			_buff.join(this->_body);
		return _buff;
	}

	void	Res::setStatus(const std::string& statusMsg)
	{ this->_statusMsg = statusMsg; }

}