/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 13:42:39 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/07 13:54:11 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResParser.hpp"

namespace http
{
	ResParser::ResParser(http::Res& res)
	 : Parser(res), _res(res)
	{
		this->_fUpdate = &Parser::checkFirstLine;
	}

	bool	ResParser::_goodFirstLine(size_t endLine)
	{
		size_t	i = 0;
		size_t	ret = 0;

		if (!(ret = this->_checkHttpVer()))
			return false;
		i += ret;
		i = this->_skipWS(i);
		if (!(ret = this->_getStatusMsg(endLine, i)))
			return false;
		return true;
	}

	size_t	ResParser::_getStatusMsg(size_t endLine, size_t i)
	{
		size_t	start = i;
		size_t	ret;
		const bool	hasCR = this->_buff[endLine - 1] == '\r';

		if (!(ret = this->_checkStatusCode(i)))
			return 0;
		i += ret;
		i = this->_skipWS(i);
		for (; i < endLine - hasCR; ++i)
			if (!this->_acceptedChar(this->_buff[i]))
				return 0;
		this->_res.setStatus(std::string(this->_buff.get_ptr() + start, i - start));
		return i;
	}

	bool	ResParser::checkHeader(size_t endLine)
	{
		if (!Parser::_checkHeader(endLine))
			this->_headerFinish = true;
		else
			this->_buff.advance(endLine + 1);
		return true;
	}

	bool	ResParser::checkFirstLine(size_t endLine)
	{
		if (this->_goodFirstLine(endLine))
		{
			this->_buff.advance(endLine + 1);
			this->_fUpdate = &Parser::checkHeader;
		}
		else
			this->_headerFinish = true;
		return true;
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
		while (this->_buff[i] == ' ')
			++i;
		return i;
	}

	size_t	ResParser::_checkStatusCode(size_t i) const
	{
		const size_t	expected_size = 3;
		try {
			uint16_t	code;

			code = std::stoi(std::string(this->_buff.get_ptr() + i, expected_size));
			if (code > 599 || code < 100)
				return 0;
		} catch (...) {
			return 0;
		}
		return expected_size;
	}

} // namespace htpp
