/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReqParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 11:52:44 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/03 09:45:41 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ReqParser.hpp"

namespace http
{
	ReqParser::ReqParser(http::Req& req)
	 : Parser(req), _req(req)
	{
		this->_fUpdate = &Parser::checkFirstLine;
	}

	bool	ReqParser::checkFirstLine(size_t endLine)
	{
		bool	ret = true;
		if (!_skipStartCRLF())
		{
			if (!this->_getStartLine(endLine))
				ret = false;
			this->_fUpdate = &Parser::checkHeader; // &(Parser::checkFirstLine) err?
		}
		this->_buff.advance(endLine + 1);
		return ret;
	}

	static size_t	_skipCRLF(ws::shared::Buffer& buff)
	{
		size_t i;

		for (i = 0; i < buff.size() && (buff[i] == '\r' || buff[i] == '\n'); ++i);
		return i;
	}

	bool	ReqParser::_skipStartCRLF(void)
	{
		size_t	i = 0;

		if (this->_req.method() == UNDEF)
		{
			i = _skipCRLF(this->_buff);
			this->_buff.advance(i);
		}
		return i;
	}

	bool	ReqParser::_getStartLine(size_t endLine)
	{
		if (endLine)
		{
			size_t index = this->_getMethod(endLine);
			if (index) // if not failed method -> UNDEF
			{
				size_t endPath = this->_buff.find(HTTPVER, endLine);
				if (endPath != std::string::npos)
					if (endPath + 8 == endLine
						|| (endPath + 9 == endLine
							&& this->_buff[endPath + 8] == '\r'))
						return this->_getPath(index, endPath);
			}
		}
		return false;
	}

	bool	ReqParser::_getPath(size_t index, size_t endPath)
	{
		size_t	_maybeEndPath;

		while (this->_buff[index] == ' ')
			++index;
		if ((_maybeEndPath = this->_checkPathValidity(index, endPath)))
		{
			while (this->_buff[--endPath] == ' ');
			++endPath;
			if (endPath == _maybeEndPath)
				this->_req.setPath(std::string(this->_buff.get_ptr() + index, endPath - index));
		}
		return !this->_req.path().empty();
	}

	size_t	ReqParser::_checkPathValidity(size_t index, size_t endPath)
	{
		if (this->_buff[index] != '/')
			return 0;
		for (; index < endPath; ++index)
			if (!this->_acceptedChar(this->_buff[index])
			|| (index && this->_buff[index - 1] == '.' && this->_buff[index] == '.')) // protect /../../
				break ;
		return index;
	}

	bool	ReqParser::_failed(void)
	{
		this->_req.setMethod(UNDEF);
		return false;
	}

	size_t	ReqParser::_getMethod(size_t endLine)
	{
		std::string	methods[] = METHODS;
		size_t	i = 0;
		
		while (i < N_METHOD
			&& this->_buff.find(methods[i].c_str(), endLine) == std::string::npos)
			++i;
		if (i >= N_METHOD)
			return this->_failed();
		this->_req.setMethod(static_cast<e_method>(i));
		return methods[i].size();
	}
} // namespace http