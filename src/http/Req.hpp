#pragma once
#include "../shared/Buffer.hpp"
#include "method.h"
#include <map>

namespace http
{
	class Req
	{
		typedef std::map<std::string, std::string>	body_m;

	private:
		char*		_raw;
		e_method	_method;
		std::string	_path;
		body_m		_body;

	public:
		Req(ws::shared::Buffer buff);
		e_method	method(void) const;
		const std::string&	path(void) const;

	private:
		std::string		_getNextHttpLine(void);
		void			_getStartLine(std::string line);
		size_t			_getMethod(std::string& line);
		void			_getBody(void);
		void			_insertBodyField(std::string& line);
	};
} // namespace http