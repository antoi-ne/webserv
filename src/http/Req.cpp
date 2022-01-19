/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:11:34 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/18 17:10:31 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Req.hpp"
#include <exception>
#include <iostream>

namespace http
{
	e_method	Req::method(void) const { return this->_method; }

	const std::string&	Req::path(void) const { return this->_path; }

	const std::string&	Req::header(const std::string& field) const
	{ return this->_header.at(field); }

	Req::Req(ws::shared::Buffer buff)
	 :	_method(), _path(), _header()
	{
		size_t	headerSize = 0;

		this->_getStartLine(this->_getNextHttpLine(buff, headerSize));
		this->_getHeader(buff, headerSize);
		this->_body.assign(buff.get_ptr() + headerSize, buff.size() - headerSize);

		std::cout << "	path: " << this->_path << std::endl;
		std::cout << "	method: " << this->_method << std::endl;
		std::cout << "		header: " << std::endl;
		for (header_m::iterator it = this->_header.begin();
			it != this->_header.end(); ++it)
			std::cout << it->first << ":" << it->second << std::endl;

		std::cout << "		body: " << std::endl;
		std::cout << this->_body.to_string() << std::endl;
	}

	std::string Req::_getNextHttpLine(const ws::shared::Buffer& buff, size_t& headerSize) const
	{
		const char*		raw = buff.get_ptr();
		std::string		line;
		size_t			startSize = headerSize;

		if (headerSize >= buff.size())
			return std::string();
		for (; raw[headerSize] != '\n' && headerSize < buff.size(); ++headerSize)
		line = std::string(raw + startSize, headerSize - startSize);
		for (; raw[headerSize] == '\n' || raw[headerSize] == '\r'; ++headerSize);
		if (headerSize == buff.size() && !line[0])
			return std::string();
		return line;
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

		while (!(line = this->_getNextHttpLine(buff, headerSize)).empty())
			this->_insertHeaderField(line);
	}

	void	Req::_insertHeaderField(std::string& line)
	{
		size_t	sep;

		sep = line.find(':', 0);
		this->_header.insert(std::pair<std::string, std::string>(line.substr(0, sep), line.substr(sep + 2)));
	}

} // namespace http