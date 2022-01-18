#include "Req.hpp"
#include <exception>
#include <iostream>

namespace http
{
	e_method	Req::method(void) const { return this->_method; }
	const std::string&	Req::path(void) const { return this->_path; }

	Req::Req(ws::shared::Buffer buff)
		: _raw(buff.get_ptr()), _method(), _path(), _body()
	{
		this->_getStartLine(this->_getNextHttpLine());
		this->_getBody();
	}

	std::string Req::_getNextHttpLine(void)
	{
		size_t lineLen = 0;
		std::string line;

		if (!this->_raw[lineLen])
			return "";
		for (; this->_raw[lineLen] != '\n' && this->_raw[lineLen]; ++lineLen)
		line = std::string(this->_raw, lineLen);
		for (; this->_raw[lineLen] == '\n' || this->_raw[lineLen] == '\r'; ++lineLen);
		this->_raw += lineLen;
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

	void	Req::_insertBodyField(std::string& line)
	{
		size_t	sep;

		sep = line.find(':', 0);
		this->_body.insert(std::pair<std::string, std::string>(line.substr(0, sep), line.substr(sep + 1)));
	}

	void	Req::_getBody(void)
	{
		std::string	line;

		while (!(line = this->_getNextHttpLine()).empty())
			this->_insertBodyField(line);
	}

} // namespace http