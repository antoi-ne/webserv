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
	bool	Req::hasHeader(void) const { return this->_hasHeader; }

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
	 	_contentLength(), _hasHeader()
	{}

	// Get buffer and update request's content
	bool	Req::update(ws::shared::Buffer& buff)
	{
		if (this->_method == UNDEF)
			return this->_checkStartLine(buff);
		this->_buff.join(buff);
		if (!this->_hasHeader)
			return this->_checkHeader();
		return this->_checkBody();
	}

	static size_t	_skipCRLF(ws::shared::Buffer& buff)
	{
		const char*	ptr = buff.get_ptr();
		size_t i;

		for (i = 0; i < buff.size() && (ptr[i] == '\r' || ptr[i] == '\n'); ++i);
		return i;
	}

	bool	Req::_checkStartLine(ws::shared::Buffer& buff)
	{
		size_t	endLine;

		if (!this->_buff.size())
		{
			size_t	i = _skipCRLF(buff);
			if (i == buff.size())
				return true;
			buff.advance(i);
		}
		this->_buff.join(buff);
		if (std::string::npos != (endLine = this->_buff.find("\r\n")))
		{
			if (!this->_getStartLine(endLine))
				return false;
			this->_buff.advance(endLine);
			return this->_checkHeader();
		}
		return true;
	}

	// Can be bigger than contentLength
	bool	Req::_checkBody(void)
	{
		if (this->_buff.size() >= this->_contentLength)
			return false;
		return true;
	}

	bool	Req::_checkHeader(void)
	{
		if (this->_buff.find("\r\n\r\n") != std::string::npos)
		{
			this->_setHeader();
			return this->_endHeader();
		}
		return true;
	}

	void	Req::_setHeader(void)
	{
		std::string	line;

		while (!(line = _getNextHeaderLine()).empty())
			this->_insertHeaderField(line);
		this->_hasHeader = true;
	}

	static inline size_t	_setContentLength(const std::string& s)
	{
		try {
			return std::stoul(s);
		} catch (...) {}
		return 0;
	}

	bool	Req::_endHeader(void)
	{
		this->_contentLength = _setContentLength(this->_header["Content-Length"]);
		if (this->_method == POST
			&& this->_contentLength > this->_buff.size())
			return true;
		return false;
	}

	bool	Req::_getStartLine(size_t endLine)
	{
		std::string	line(this->_buff.get_ptr(), endLine);

		if (!line.empty())
		{
			size_t index = this->_getMethod(line);
			if (index) // if not failed method -> UNDEF
			{
				size_t endPath = line.find(HTTPVER, index);
				if (endPath != std::string::npos) // if fail not good version path empty
				{
					while (line[index] == ' ')
						++index;
					while (line[endPath - 1] == ' ' && endPath > index)
						--endPath;
					this->_path = line.substr(index, endPath - index);
					return !this->_path.empty();
				}
			}
		}
		return false;
	}

	// return the line before the next "\r\n" inside a Buffer 
	std::string	Req::_getNextHeaderLine(void)
	{
		std::string		raw = this->_buff.get_ptr();
		std::string		line;
		size_t			EOL;
		
		EOL = raw.find("\r\n");
		if (EOL != std::string::npos)
			line = raw.substr(0, EOL);
		this->_buff.advance(EOL + 2);
		return line;
	}

	static inline bool	_compareMethod(const std::string& line, const std::string& method)
	{ return line.compare(0, method.size(), method) == 0; }


	size_t	Req::_getMethod(std::string& line)
	{
		std::string	methods[] = METHODS;
		size_t	i = 0;

		while (i < N_METHOD && !_compareMethod(line, methods[i]))
			++i;
		if (i == N_METHOD)
		{
			this->_method = UNDEF;
			return 0;
		}
		this->_method = static_cast<e_method>(i);
		return methods[i].size();
	}

	void	Req::_insertHeaderField(std::string& line)
	{
		size_t	i = 0;
		size_t	keyEnd = 0;

		for (; i < line.size() && line[i] != ':'; ++i);
		keyEnd = i;
		if (i < line.size())
		{
			++i;
			for (; i < line.size() && line[i] == ' '; ++i);
			this->_header.insert(std::make_pair(line.substr(0, keyEnd), line.substr(i)));
		}
	}

} // namespace http