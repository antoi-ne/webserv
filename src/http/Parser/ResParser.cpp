/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 13:42:39 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/01 15:54:02 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResParser.hpp"

namespace http
{
	ResParser::ResParser(http::Res& res)
	 : Parser(res)
	{}

	bool	ResParser::_checkFirstLine(size_t endLine)
	{
		if (!this->_checkHttpVer())
			return false;
		this->_skipWS();
		if (!this->_checkStatusCode())
			return false;
		this->_skipWS();
		for (size_t i = 0; i < endLine; ++i)
			if (!this->_acceptedChar(this->_buff[i]))
				return false;
		return true;
	}

	bool	ResParser::_checkHttpVer(void)
	{
		std::string	httpVer(HTTPVER);

		size_t start = this->_buff.find(httpVer);

		if (start == 0) // wrong HTTP version
			return false;
		this->_buff.advance(httpVer.size());
		return true;
	}

	void	ResParser::_skipWS(void)
	{
		size_t	i = 0;

		for (; this->_buff[i] == ' '; ++i);
		this->_buff.advance(i);
	}

	bool	ResParser::_checkStatusCode(void) const
	{
		try {
			std::stoi(std::string(this->_buff.get_ptr(), 3));
		} catch (...) {
			return false;
		}
		return true;
	}

} // namespace htpp
