/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 11:22:56 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/01 14:43:23 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

namespace http
{
	Parser::Parser(http::Message& msg)
	 : _msg(msg), _buff(), _headerFinish(), _fUpdate(&Parser::_updateFirstLine)
	{}

	http::Message&	Parser::_getMsg(void)
	{ return this->_msg; }

	bool	Parser::update(const ws::shared::Buffer& buff)
	{
		this->_buff.join(buff);
		if (!this->_headerFinish)
			this->_updateIfCRLF();
		return this->_isNotFinish();
	}

	bool	Parser::_isNotFinish(void)
	{
		if (this->_headerFinish)
		{
			if (this->_msg.contentLength() != std::string::npos
			&&  this->_buff.size() >= this->_msg.contentLength())
				return false;
			if (this->_msg.header()["transfer-encoding"] == "chunked")
				return false;
		}
		return true;
	}

	bool	Parser::_updateIfCRLF(void)
	{
		size_t endLine = this->_buff.find('\n');

		while (endLine != std::string::npos && !this->_headerFinish)
		{
			if ((this->*_fUpdate)(endLine) == false) // error case
				return false;
			endLine = this->_buff.find('\n');
		}
		return true;
	}

	bool	Parser::_updateFirstLine(size_t endLine)
	{
		if (!this->_checkFirstLine(endLine))
			return false;
		this->_fUpdate = &Parser::_checkHeader;
		return true;
	}

	bool	Parser::_acceptedChar(const char c) const
	{ return (c > 32 && c < 127); }

	static inline size_t	_setContentLength(const std::string& s)
	{
		try {
			return std::stoul(s);
		} catch (...) {}
		return 0;
	}

	void	Parser::_endHeader(void)
	{
		this->_headerFinish = true;
		this->_msg.setContentLength(
			_setContentLength(this->_msg.header()["Content-Length"]));
	}

	// TODO Check host
	bool	Parser::_checkHeader(size_t endLine)
	{
		if (endLine == 0 || (endLine == 1 && this->_buff[0] == '\r'))
			this->_endHeader();
		else
			if (!this->_setHeader(endLine))
				return false;
		this->_buff.advance(endLine + 1);
		return true;
	}

	bool	Parser::_setHeader(size_t endLine)
	{
		int		hasCR = 0;
		size_t	i = 0;
		size_t	keyEnd = 0;

		if (this->_buff[endLine - 1] == '\r')
			hasCR = 1;
		while (this->_buff[i] != ':'
			&& this->_acceptedChar(this->_buff[i]))
			++i;
		if (i != 0)
		{
			if (this->_buff[i] == ':')
			{
				keyEnd = i++;
				for (; i < endLine && this->_buff[i] <= ' '; ++i); // not sure skip space before value
				this->_msg.header().insert(
				std::make_pair(
					std::string(this->_buff.get_ptr(), keyEnd),
					std::string(this->_buff.get_ptr() + i, endLine - i - hasCR)
				));
			}
			else if (hasCR)
			{
				if (i != endLine - 1)
					return false;
			}
			else if (i != endLine)
				return false;
			return true;
		}
		return false;
	}
	
} // namespace http
