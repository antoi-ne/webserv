/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:11:34 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/20 16:50:20 by vneirinc         ###   ########.fr       */
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

	const std::string&	Req::header(const std::string& field)
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
		this->_buff.join(buff);
		if (!this->_hasHeader)
			return this->_checkHeader();
		return this->_checkBody(buff);
	}

	// Can be bigger than contentLength
	bool	Req::_checkBody(ws::shared::Buffer& buff)
	{
		if (this->_buff.size() >= this->_contentLength)
			return false;
		return true;
	}

	bool	Req::_checkHeader(void)
	{
		std::string str = this->_buff.to_string();

		size_t	EOH = str.find("\r\n\r\n");
		if (EOH != std::string::npos)
		{
			this->_setHeader();
			return this->_endHeader();
		}
		return true;
	}

	void	Req::_setHeader(void)
	{
		this->_getStartLine();
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
		this->_contentLength = _setContentLength(this->header("Content-Length"));
		if (this->_method == POST
			&& this->_contentLength > this->_buff.size())
			return true;
		return false;
	}

	void	Req::_getStartLine(void)
	{
		std::string	line = _getNextHeaderLine();

		if (line.empty())
			throw std::exception();

		size_t index = this->_getMethod(line);

		size_t endPath = line.find(HTTPVER, index);

		if (endPath == std::string::npos)
			throw std::exception();

		this->_path = line.substr(index, endPath - index);
		if (this->_path.empty())
			throw std::exception();
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

		for (; i < N_METHOD && !_compareMethod(line, methods[i]); ++i)
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
		size_t	sep = 0;

		for (; sep < line.size() && line[sep] != ':'; ++sep);
		if (line[sep] == ':')
			this->_header.insert(std::pair<std::string, std::string>(line.substr(0, sep), line.substr(sep + 2)));
	}

} // namespace http