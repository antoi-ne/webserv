/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 11:22:56 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/02 12:27:28 by vneirinc         ###   ########.fr       */
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

	bool	Parser::headerFinish(void) const
	{ return this->_headerFinish; }

	bool	Parser::update(const ws::shared::Buffer& buff)
	{
		if (!this->_headerFinish)
		{
			this->_buff.join(buff);
			if (!this->_updateIfCRLF())
				return false;
		}
		else
			this->_msg.body().join(buff);
		return this->_isNotFinish();
	}

	bool	Parser::_chunkedSize(size_t endLine, size_t& chunkSize) const
	{
		bool	hasCR = this->_buff[endLine - 1] == '\r';

		if (endLine == 0 || (endLine == 1 && hasCR))
			return false;
		try {
			chunkSize = std::stoul(std::string(this->_buff.get_ptr(), endLine - hasCR), nullptr, 16);
		} catch (...) {
			return false;
		};
		return true;
	}

	bool	Parser::_chunkedContent(size_t endLine, size_t& chunkSize)
	{
		bool	hasCR = this->_buff[endLine - 1] == '\r';
		size_t	_chunkSize = chunkSize;

		chunkSize = std::string::npos;
		if (_chunkSize == 0 && (endLine == 0 || (endLine == 1 && hasCR)))
		{
			this->_headerFinish = true;
			this->_msg.setContentLength(this->_msg.body().size());
		}
		else if (endLine - hasCR != _chunkSize)
			return false;
		else
			this->_msg.body().join(ws::shared::Buffer(this->_buff.get_ptr(), _chunkSize));
		return true;
	}

	bool	Parser::_unchunkedBody(size_t endLine)
	{
		static size_t	chunkSize = std::string::npos;

		if (chunkSize == std::string::npos)
			return this->_chunkedSize(endLine, chunkSize);
		return this->_chunkedContent(endLine, chunkSize);
	}

	bool	Parser::_isNotFinish(void)
	{
		if (this->_msg.contentLength() != std::string::npos
			&& this->_msg.body().size() >= this->_msg.contentLength())
			return false;
		return true;
	}

	bool	Parser::_updateIfCRLF(void)
	{
		size_t endLine = this->_buff.find('\n');

		while (endLine != std::string::npos && !this->_headerFinish)
		{
			if ((this->*_fUpdate)(endLine) == false) // error case
				return false;
			this->_buff.advance(endLine + 1);
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
		if (this->_msg.header()["transfer-encoding"] == "chunked")
			this->_fUpdate = &Parser::_unchunkedBody;
		else
		{
			this->_msg.body().join(this->_buff);
			this->_headerFinish = true;
			this->_msg.setContentLength(
				_setContentLength(this->_msg.header()["Content-Length"]));
		}
	}

	// TODO Check host
	bool	Parser::_checkHeader(size_t endLine)
	{
		if (endLine == 0 || (endLine == 1 && this->_buff[0] == '\r'))
			this->_endHeader();
		else
			if (!this->_setHeader(endLine))
				return false;
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
