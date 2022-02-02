/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 13:42:39 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/02 09:53:40 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResParser.hpp"

namespace http
{
	ResParser::ResParser(http::Res& res)
	 : Parser(res)
	{}

	bool	ResParser::_goodFirstLine(size_t endLine)
	{
		size_t	i = 0;
		size_t	ret = 0;

		if (!(ret = this->_checkHttpVer()))
			return false;
		i += ret + this->_skipWS();
		if (!(ret = this->_checkStatusCode()))
			return false;
		i += ret + this->_skipWS();
		for (; i < endLine; ++i)
			if (!this->_acceptedChar(this->_buff[i]))
				return false;
		return true;
	}

	bool	ResParser::_checkFirstLine(size_t endLine)
	{
		if (!this->_goodFirstLine(endLine))
			return false;
		this->_buff.advance(endLine + 1);
		return true;
	}

	size_t	ResParser::_checkHttpVer(void)
	{
		std::string	httpVer(HTTPVER);

		size_t start = this->_buff.find(httpVer);

		if (start == 0) // wrong HTTP version
			return 0;
		return httpVer.size();
	}

	size_t	ResParser::_skipWS(void) const
	{
		size_t	i = 0;

		for (; this->_buff[i] == ' '; ++i);
		return i;
	}

	size_t	ResParser::_checkStatusCode(void) const
	{
		const size_t	expected_size = 3;
		try {
			std::stoi(std::string(this->_buff.get_ptr(), expected_size));
		} catch (...) {
			return 0;
		}
		return expected_size;
	}

} // namespace htpp
