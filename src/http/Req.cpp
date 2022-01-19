/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:11:34 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/19 15:09:20 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Req.hpp"
#include <exception>
#include <iostream>

namespace http
{

	bool	Req::updateBody(ws::shared::Buffer& buff)
	{
		if (this->_body.size() + buff.size() <= this->_contentLength)
			this->_body.join(buff);
		return this->_body.size() == this->_contentLength;
	}

	e_method	Req::method(void) const { return this->_method; }

	const std::string&	Req::path(void) const { return this->_path; }

	const std::string&	Req::header(const std::string& field) const
	{ return this->_header.at(field); }

	Req::Req(ws::shared::Buffer buff)
	 :	_method(), _path(), _header(), _contentLength()
	{
		size_t	headerSize = 0;

		std::cout << buff.to_string() << std::endl;
		this->_getStartLine(this->_getHeaderLine(buff, headerSize));
		this->_getHeader(buff, headerSize);
		this->_getBody(buff.get_ptr() + headerSize, buff.size() - headerSize);
	}

	static inline bool _is_CRLF(char c) { return (c == '\r' || c == '\n'); }

	std::string Req::_getHeaderLine(const ws::shared::Buffer& buff, size_t& headerSize) const
	{
		const char*		raw = buff.get_ptr();
		std::string		line;
		size_t			startSize = headerSize;

		if (_is_CRLF(raw[headerSize]) || headerSize >= buff.size())
			return std::string();
		for (; !_is_CRLF(raw[headerSize]) && headerSize < buff.size(); ++headerSize);
		line = std::string(raw + startSize, headerSize - startSize);
		++headerSize;
		if (_is_CRLF(raw[headerSize]))
			++headerSize;
		if (headerSize == buff.size() && !line[0])
			return std::string();
		return line;
	}

	void	Req::_getBody(char* bodyBuff, size_t bodySize)
	{ 
		header_m::iterator it = this->_header.find("Content-Length");

		if (it != this->_header.end())
		{
			this->_contentLength = std::stoul(it->second);
			if (this->_contentLength <= bodySize)
				this->_body.assign(bodyBuff, bodySize);
			std::cout << this->_body.to_string();
		}
		this->_contentLength = 0;
	}

	void	Req::_getStartLine(std::string line)
	{
		size_t index = this->_getMethod(line);
		size_t endPath = line.find(HTTPVER, index);
		if (endPath == std::string::npos)
			throw std::exception();
		this->_path = line.substr(index, endPath - index);
	}

	static inline bool	_compareMethod(std::string line, std::string method)
	{ return line.compare(0, method.size(), method) == 0; }


	size_t	Req::_getMethod(std::string& line)
	{
		char*	methods[] = METHODS;
		size_t	i = 0;

		while (!_compareMethod(line, methods[i]))
		{
			++i;
			if (!methods[i])
				throw std::exception();
		}
		this->_method = static_cast<e_method>(i);
		return strlen(methods[i]);
	}

	void	Req::_getHeader(const ws::shared::Buffer& buff, size_t& headerSize)
	{
		std::string	line;

		while (!(line = this->_getHeaderLine(buff, headerSize)).empty())
			this->_insertHeaderField(line);
	}

	void	Req::_insertHeaderField(std::string& line)
	{
		size_t	sep;

		sep = line.find(':', 0);
		this->_header.insert(std::pair<std::string, std::string>(line.substr(0, sep), line.substr(sep + 2)));
	}

} // namespace http