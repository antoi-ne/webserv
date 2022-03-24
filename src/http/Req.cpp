/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 11:17:46 by vneirinc          #+#    #+#             */
/*   Updated: 2022/03/24 09:06:40 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Req.hpp"

namespace	http
{
	bool	Req::_hasBody(e_method method) { return method == POST || method == PUT; }

	e_method	Req::method(void) const { return this->_method; }

	const std::string&	Req::path(void) const { return this->_path; }

	void	Req::setPath(const std::string& path) { this->_path = path; }

	void	Req::setMethod(e_method method) { this->_method = method; }

	time_t	Req::getCreatedTime(void) const { return this->_createdTime; }

	ws::shared::Buffer	Req::getReq(void) const
	{
		const std::string	methods[] = METHODS;
		const std::string	crlf("\r\n"); 
		ws::shared::Buffer	buff(methods[this->_method]);

		buff.join(this->_path);
		buff.join(" "HTTPVER"\r\n");

		if (this->_contentLength == std::string::npos)
		{
			buff.join("Content-Length: ");
			buff.join(std::to_string(this->_body.size()));
			buff.join(crlf);
		}

		for (header_m::const_iterator it = this->_header.begin(); it != this->_header.end(); ++it)
			buff.join(it->first + std::string(": ") + it->second + crlf);
		buff.join(crlf);

		if (this->_body.size())
			buff.join(this->_body);
		return buff;
	}

	Req::Req(void)
	 : Message(), _method(UNDEF), _path(), _createdTime(time(0))
	{}
}