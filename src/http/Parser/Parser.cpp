/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 11:22:56 by vneirinc          #+#    #+#             */
/*   Updated: 2022/03/21 15:45:55 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

namespace http
{
	Parser::Parser(http::Message& msg)
	 : _msg(msg), _buff(), _headerFinish(), _fUpdate(&Parser::checkHeader)
	{}

	Parser::Parser(http::Message& msg, bool (Parser::*fUpdate)(size_t))
	 : _msg(msg), _buff(), _headerFinish(), _fUpdate(fUpdate)
	{}

	http::Message&	Parser::_getMsg(void)
	{ return this->_msg; }

	bool	Parser::headerFinish(void) const
	{ return this->_headerFinish; }

	bool	Parser::update(const ws::shared::Buffer& buff)
	{
		if (!this->_addBuffer(buff)
			|| (!this->_headerFinish && !this->_updateIfCRLF()))
			return false;
		return this->_isNotFinish();
	}

	bool	Parser::_addBuffer(const ws::shared::Buffer& buff)
	{
		if (this->_msg.contentLength() == std::string::npos)
			this->_buff.join(buff);
		else if (this->_msg.body().size() + buff.size() > this->_msg.contentLength())
		{
			this->_headerFinish = false;
			return false;
		}
		else
			this->_msg.body().join(buff);
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

	bool	Parser::_isNotFinish(void)
	{
		if (this->_headerFinish)
		{
			if (this->_msg.contentLength() == std::string::npos)
			{
				if (!this->_unchunkedBody())
				{
					this->_headerFinish = false;
					return false;
				}
			}
			if (this->_msg.body().size() >= this->_msg.contentLength())
			{
				if (this->_msg.body().size() > this->_msg.contentLength())
					this->_headerFinish = false;
				return false;
			}
		}
		return true;
	}

	bool	Parser::_chunkedSize(size_t endLine, size_t& chunkSize) const
	{
		if (endLine == 0
			|| (endLine == 1 && this->_buff[0] == '\r')) // just crlf
			return false;
		bool hasCR = this->_buff[endLine - 1] == '\r';
		try {
			chunkSize = std::stoul(std::string(this->_buff.get_ptr(), endLine - hasCR), nullptr, 16);
		} catch (...) {
			return false;
		};
		return true;
	}

	bool	Parser::_chunkedContent(size_t& chunkSize)
	{
		size_t	size;

		if (this->_buff.size() < chunkSize)
			size = this->_buff.size();
		else
			size = chunkSize;
		this->_msg.body().join(this->_buff.get_ptr(), size);
		this->_buff.advance(size);
		chunkSize -= size;
		if (chunkSize == 0)
			return true;
		return false;
	}

	bool	Parser::_unchunkedBody(void)
	{
		static bool			needCRLF = false;
		static size_t		chunkSize = std::string::npos;

		while (this->_buff.size() && (chunkSize != 0 || needCRLF))
		{
			if (needCRLF)
			{
				bool	hasCR = this->_buff[0] == '\r';

				needCRLF = false;
				if (this->_buff[0] == '\n' || (hasCR && this->_buff[1] == '\n'))
				{
					chunkSize = std::string::npos;
					this->_buff.advance(1 + hasCR);
				}
				else
				{
					chunkSize = std::string::npos;
					return false;
				}
			}
			if (chunkSize == std::string::npos)
			{
				size_t	endLine = this->_buff.find('\n');

				if (endLine == std::string::npos)
					return true;
				if (!this->_chunkedSize(endLine, chunkSize))
				{
					chunkSize = std::string::npos;
					return false;
				}
				this->_buff.advance(endLine + 1);
			}
			else
				needCRLF = this->_chunkedContent(chunkSize);
		}
		if (this->_buff.size())
		{
			chunkSize = std::string::npos;
			if (this->_buff[0] == '\n' || (this->_buff[0] == '\r' && this->_buff[1] == '\n'))
				this->_msg.setContentLength(this->_msg.body().size());
			else
				return false;
		}
		return true;
	}

	bool	Parser::_acceptedChar(const char c) const
	{ return (c > 31 && c < 127); }

	static inline size_t	_setContentLength(const std::string& s)
	{
		try {
			return std::stoul(s);
		} catch (...) {}
		return 0;
	}

	void	Parser::_endHeader(size_t endLine)
	{
		if (this->_msg.header()["transfer-encoding"] != "chunked")
		{
			this->_msg.body().join(
				this->_buff.get_ptr() + endLine + 1,
				this->_buff.size() - (endLine + 1)
			);
			this->_msg.setContentLength(
				_setContentLength(this->_msg.header("content-length")));
		}
		this->_headerFinish = true;
		this->_msg.header().erase("transfer-encoding");
	}

	bool	Parser::_checkHeader(size_t endLine)
	{
		bool	ret = true;

		if (endLine == 0 || (endLine == 1 && this->_buff[0] == '\r'))
		{
			if (this->_msg.header("host") == "")
				ret = false;
			else
				this->_endHeader(endLine);
		}
		else
			if (!this->_setHeader(endLine))
				ret = false;
		return ret;
	}

	bool	Parser::_acceptedKey(const std::string& key, const std::string& value) const
	{
		if (key == "host" && value.empty())
			return false;
		return true;
	}

	bool	Parser::_setHeader(size_t endLine)
	{
		int		hasCR = 0;
		size_t	i = 0;

		if (this->_buff[endLine - 1] == '\r')
			hasCR = 1;
		while (this->_buff[i] != ':'
			&& this->_buff[i] != ' '
			&& this->_acceptedChar(this->_buff[i]))
			++i;
		if (i != 0)
		{
			std::string	key;
			std::string	val;

			key = std::string(this->_buff.get_ptr(), i);
			if (this->_buff[i] == ':')
			{
				i++;
				for (; i < endLine && this->_buff[i] == ' '; ++i);
				val = std::string(this->_buff.get_ptr() + i, endLine - i - hasCR);
				this->_msg.header().insert(std::make_pair(key, val));
			}
			else if (hasCR)
			{
				if (i != endLine - 1)
					return false;
			}
			else if (i != endLine)
				return false;
			return _acceptedKey(key, val);
		}
		return false;
	}
	
} // namespace http
