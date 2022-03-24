/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Res.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:15:36 by vneirinc          #+#    #+#             */
/*   Updated: 2022/03/24 09:15:08 by vneirinc         ###   ########.fr       */
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
		this->_header.insert(std::make_pair("Content-Type", "text/plain"));
	}

	void	Res::_getTime(ws::shared::Buffer& buff) const
	{
		time_t	t = time(0);
		tm		*ltm = gmtime(&t);
		char	date_buff[256];

		size_t s = strftime(date_buff, 255, DATE_FORMAT, ltm);
		date_buff[s] = '\0';
		buff.join(date_buff, s);
	}

	const std::string	Res::_getStatus(void)
	{
		std::string			status;
		header_m::iterator	it = this->_header.find("status");

		if (it == this->_header.end())
			status = this->_statusMsg;
		else
		{
			status = it->second;
			this->_header.erase(it);
		}
		return status;
	}

	ws::shared::Buffer Res::get_res()
	{
		ws::shared::Buffer	buff;
		const std::string	crlf("\r\n"); 

		buff.join(HTTPVER" ");
		buff.join(this->_getStatus());
		buff.join(crlf);

		buff.join("Server: "SERVER"\r\n");

		buff.join("Date: ");
		this->_getTime(buff);
		buff.join(crlf);

		this->_header.erase("content-Length");
		buff.join("Content-Length: ");
		buff.join(std::to_string(this->_body.size()) + crlf);

		for (header_m::const_iterator it = this->_header.begin(); it != this->_header.end(); ++it)
			buff.join((it->first + std::string(": ") + it->second + crlf));
		buff.join(crlf);

		if (this->_body.size())
			buff.join(this->_body);
		return buff;
	}

	void	Res::setStatus(const std::string& statusMsg)
	{ this->_statusMsg = statusMsg; }

}