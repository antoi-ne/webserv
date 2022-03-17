/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 09:05:31 by vneirinc          #+#    #+#             */
/*   Updated: 2022/03/17 11:42:10 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Message.hpp"

namespace	http
{
	Message::header_m&	Message::header(void)
	{ return this->_header; }

	const Message::header_m&	Message::header(void) const
	{ return this->_header; }

	const std::string	Message::header(const std::string field) const
	{
		header_m::const_iterator	it = this->_header.find(field);

		return it != this->_header.end() ? it->second : "";
	}

	const ws::shared::Buffer&	Message::body(void) const
	{ return this->_body; }

	size_t	Message::contentLength(void) const
	{ return this->_contentLength; }

	void	Message::setContentLength(size_t contentLength)
	{ this->_contentLength = contentLength; }

	ws::shared::Buffer&	Message::body(void)
	{ return this->_body; }

	Message::Message(void)
	 : _header(), _body(), _contentLength(std::string::npos)
	{}

	Message::~Message(void) {}
}