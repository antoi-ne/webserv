/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ReqParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 11:49:26 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/01 14:11:06 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Parser.hpp"
#include "../Req.hpp"

namespace http
{
	class ReqParser : public Parser
	{
	private:
		http::Req&		_req;
	public:
		ReqParser(http::Req& req);

	private:
		virtual bool	_checkFirstLine(size_t endLine);
		size_t			_getMethod(size_t endLine);
		bool			_getPath(size_t index, size_t endPath);
		size_t			_checkPathValidity(size_t index, size_t endPath);
		bool			_skipStartCRLF(void);
		bool			_getStartLine(size_t endLine);
		bool			_failed(void);
	};
	
} // namespace http
