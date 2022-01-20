/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:11:34 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/20 10:49:05 by vneirinc         ###   ########.fr       */
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

	bool	Req::finish(void) const { return !this->_update; }

	// Take Buffer with at least HTTP start-line
	Req::Req(ws::shared::Buffer& buff)
	 :	_method(UNDEF), _path(), _header(), _body(), _contentLength(),
	 	_update(&Req::_updateHeader)
	{
		size_t size = 0;

		this->_getStartLine(this->_getHeaderLine(buff, size));
		this->_updateHeader(buff.advance(size));
	}

	// return the line before the next "\r\n" inside a Buffer 
	std::string	Req::_getHeaderLine(const ws::shared::Buffer& buff, size_t& headerSize) const
	{
		const char*		raw = buff.get_ptr();
		std::string		line;
		size_t			startSize = headerSize;

		for (; raw[headerSize] != '\r' && headerSize < buff.size(); ++headerSize);
		line = std::string(raw + startSize, headerSize - startSize);
		if (headerSize == buff.size()) // NO /r/n
			throw std::exception();
		++headerSize; // skip /r
		++headerSize; // skip /n
		if (headerSize >= buff.size() && !line[0])
			return std::string();
		return line;
	}

	void	Req::_getStartLine(std::string line)
	{
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

	static inline bool	_compareMethod(const std::string& line, const std::string& method)
	{ return line.compare(0, method.size(), method) == 0; }


	size_t	Req::_getMethod(std::string& line)
	{
		std::string	methods[] = METHODS;
		size_t	i = 0;

		while (!_compareMethod(line, methods[i]))
		{
			++i;
			if (methods[i].empty())
				throw std::exception();
		}
		this->_method = static_cast<e_method>(i);
		return methods[i].size();
	}

	// Get buffer and update request's content
	// _update = _updateHeader > _update = _updateBody > _update = NULL
	// _update = _updateHeader > _update = NULL (if no body)
	bool	Req::update(ws::shared::Buffer& buff)
	{
		if (this->_update != NULL)
		{
			(this->*_update)(buff);
			return true;
		}
		return false;
	}

	static inline size_t	_setContentLength(const std::string& s)
	{
		return std::stoul(s);
	}

	void	Req::_endHeader(ws::shared::Buffer& buff)
	{
		if ((this->_contentLength = _setContentLength(this->header("Content-Length"))) > 0)
		{
			this->_update = &Req::_updateBody;
			this->_updateBody(buff);
		}
		else
			this->_update = NULL;
	}

	// Update header content
	void	Req::_updateHeader(ws::shared::Buffer& buff)
	{
		size_t			size;
		std::string		line;

		while (!(line = this->_getHeaderLine(buff, size)).empty())
		{
			if (line[0] == '\r')
				return _endHeader(buff.advance(size + 2));
			this->_insertHeaderField(line);
		}
	}

	void	Req::_insertHeaderField(std::string& line)
	{
		size_t	sep = 0;

		for (; sep < line.size() && line[sep] != ':'; ++sep);
		this->_header.insert(std::pair<std::string, std::string>(line.substr(0, sep), line.substr(sep + 2)));
	}

	// Update body content
	// Can be bigger than contentLength
	void	Req::_updateBody(ws::shared::Buffer& buff)
	{
		this->_body.join(buff);
		if (this->_body.size() >= this->_contentLength)
			this->_update = NULL;
	}

} // namespace http