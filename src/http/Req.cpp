/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:11:34 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/23 10:15:51y vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Req.hpp"
#include <exception>
#include <iostream>

namespace http
{ 
	bool	Req::hasHeader(void) const { return this->_headerFinish; }

	e_method	Req::method(void) const { return this->_method; }

	const std::string&	Req::path(void) const { return this->_path; }

	Req::header_m&	Req::header(void)
	{ return this->_header; }

	const Req::header_m&	Req::header(void) const
	{ return this->_header; }

	const std::string&	Req::header(const std::string field)
	{ return this->_header[field]; }

	const ws::shared::Buffer&	Req::body(void) const
	{ return this->_buff; }

	Req::Req(void)
	 :	_method(UNDEF), _path(), _header(), _buff(),
	 	_contentLength(), _headerFinish(), _check_line(&Req::_checkStartLine)
	{}

	bool	Req::_isNotFinish(void) const
	{
		if (this->_headerFinish)
		{
			if (this->_method == POST)
				if (this->_contentLength > this->_buff.size())
					return true;
			return false;
		}
		return true;
	}

	bool	Req::update(ws::shared::Buffer& buff)
	{
		this->_buff.join(buff);
		if (!this->_updateIfCRLF())
			return false;
		return this->_isNotFinish();
	}

	bool	Req::_updateIfCRLF(void)
	{
		size_t endLine = this->_buff.find('\n');

		while (endLine != std::string::npos && !this->_headerFinish)
		{
			if ((this->*_check_line)(endLine) == false) // error case
				return false;
			endLine = this->_buff.find('\n');
		}
		return true;
	}

	bool	Req::_checkStartLine(size_t endLine)
	{
		if (!this->_skipStartCRLF())
		{
			if (!this->_getStartLine(endLine))
				return false;
			this->_buff.advance(endLine + 1);
			this->_check_line = &Req::_checkHeader;
		}
		return true;
	}

	static size_t	_skipCRLF(ws::shared::Buffer& buff)
	{
		size_t i;

		for (i = 0; i < buff.size() && (buff[i] == '\r' || buff[i] == '\n'); ++i);
		return i;
	}

	bool	Req::_skipStartCRLF(void)
	{
		size_t	i = 0;

		if (this->_method == UNDEF)
		{
			i = _skipCRLF(this->_buff);
			this->_buff.advance(i);
		}
		return i;
	}

	bool	Req::_getStartLine(size_t endLine)
	{
		if (endLine)
		{
			size_t index = this->_getMethod(endLine);
			if (index) // if not failed method -> UNDEF
			{
				size_t endPath = this->_buff.find(HTTPVER, endLine);
				if (endPath != std::string::npos) // if fail not good version path empty
					return this->_getPath(index, endPath);
			}
		}
		return false;
	}

	bool	Req::_getPath(size_t index, size_t endPath)
	{
		size_t	_maybeEndPath;

		while (this->_buff[index] == ' ')
			++index;
		if ((_maybeEndPath = this->_checkPathValidity(index, endPath)))
		{
			while (this->_buff[--endPath] == ' ');
			++endPath;
			if (endPath == _maybeEndPath)
				this->_path = std::string(this->_buff.get_ptr() + index, endPath - index);
		}
		return !this->_path.empty();
	}

	static inline bool	_acceptedChar(const char c)
	{ return (c > 32 && c < 127); }

	size_t	Req::_checkPathValidity(size_t index, size_t endPath)
	{
		if (this->_buff[index] != '/')
			return 0;
		for (; index < endPath; ++index)
			if (!_acceptedChar(this->_buff[index]))
				break ;
		return index;
	}

	static inline size_t	_setContentLength(const std::string& s)
	{
		try {
			return std::stoul(s);
		} catch (...) {}
		return 0;
	}

	// TODO Check host
	bool	Req::_checkHeader(size_t endLine)
	{
		if (endLine == 0 || (endLine == 1 && this->_buff[0] == '\r'))
		{
			this->_headerFinish = true;
			this->_contentLength = _setContentLength(this->_header["Content-Length"]);
			this->_check_line = NULL;
		}
		else
			if (!this->_setHeader(endLine))
				return false;
		this->_buff.advance(endLine + 1);
		return true;
	}

	bool	Req::_failed(void)
	{
		this->_method = UNDEF;
		return false;
	}

	bool	Req::_setHeader(size_t endLine)
	{
		size_t	i = 0;
		size_t	keyEnd = 0;

		while (i < endLine && this->_buff[i] != ':' && _acceptedChar(this->_buff[i]))
			++i;
		if (i == 0 || (this->_buff[i] != ':' && endLine - i > 1)) // currently accept any unaccepted char before \n at end normal \n or \r\n
			return this->_failed();
		keyEnd = i++;
		for (; i < endLine && this->_buff[i] <= ' '; ++i); // not sure skip space before value
		this->_header.insert(
			std::make_pair(
				std::string(this->_buff.get_ptr(), keyEnd),
				std::string(this->_buff.get_ptr() + i, endLine - i)
		));
		return true;
	}

	size_t	Req::_getMethod(size_t endLine)
	{
		std::string	methods[] = METHODS;
		size_t	i = 0;
		
		while (i < N_METHOD
			&& this->_buff.find(methods[i].c_str(), endLine) == std::string::npos)
			++i;
		if (i >= N_METHOD)
			return this->_failed();
		this->_method = static_cast<e_method>(i);
		return methods[i].size();
	}

} // namespace http