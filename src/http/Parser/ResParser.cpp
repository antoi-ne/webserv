/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancoulon <ancoulon@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 13:42:39 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/02 16:40:21 by ancoulon         ###   ########.fr       */
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
		i += ret + this->_skipWS(i);
		if (!(ret = this->_checkStatusCode(i)))
			return false;
		i += ret + this->_skipWS(i);
		for (; i < endLine; ++i)
			if (!this->_acceptedChar(this->_buff[i]))
				return false;
		return true;
	}

	bool	ResParser::_checkFirstLine(size_t endLine)
	{
		return this->_goodFirstLine(endLine);
	}

	size_t	ResParser::_checkHttpVer(void)
	{
		std::string	httpVer(HTTPVER);

		size_t start = this->_buff.find(httpVer);

		if (start == 0) // wrong HTTP version
			return httpVer.size();
		return 0;
	}

	size_t	ResParser::_skipWS(size_t i) const
	{
		for (; this->_buff[i] == ' '; ++i);
		return i;
	}

	size_t	ResParser::_checkStatusCode(size_t i) const
	{
		const size_t	expected_size = 3;
		try {
			std::cout << "test: " << std::string(this->_buff.get_ptr() + i, expected_size) << std::endl;
			std::stoi(std::string(this->_buff.get_ptr() + i, expected_size));
		} catch (...) {
			return 0;
		}
		return expected_size;
	}

} // namespace htpp
