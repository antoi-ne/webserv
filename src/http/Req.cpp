/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 11:17:46 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/01 17:04:18 by vneirinc         ###   ########.fr       */
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

	ws::shared::Buffer	Req::getReq(void) const
	{
		std::string		methods[] = METHODS;
		std::string		buff;
		std::string		crlf("\r\n"); 

		buff += methods[this->_method];
		buff += this->_path;
		buff += " ";
		buff += HTTPVER"\r\n";

		for (header_m::const_iterator it = this->_header.begin(); it != this->_header.end(); ++it)
			buff += (it->first + std::string(": ") + it->second + crlf);
		buff += crlf;
		ws::shared::Buffer	_buff(buff);

		if (this->_body.size())
			_buff.join(this->_body);
		return _buff;
	}

	Req::Req(void)
	 : Message(), _method(), _path()
	{}
}