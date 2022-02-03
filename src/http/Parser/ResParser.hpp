/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 13:43:01 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/03 09:28:25 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Parser.hpp"
#include "../Res.hpp"

namespace http
{
	class ResParser : public Parser
	{
	private:
		http::Res&		_res;
	public:
		ResParser(http::Res& res);

		virtual bool	checkFirstLine(size_t endLine);
	private:
		bool	_goodFirstLine(size_t endLine);
		size_t	_checkHttpVer(void);
		size_t	_getStatusMsg(size_t endLine, size_t i);
		size_t	_checkStatusCode(size_t i) const;
		size_t	_skipWS(size_t i) const;
	};
	
} // namespace htpp
