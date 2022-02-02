/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 13:43:01 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/02 09:50:50 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Parser.hpp"
#include "../Res.hpp"

namespace http
{
	class ResParser : public Parser
	{
	public:
		ResParser(http::Res& res);
	private:
		virtual bool	_checkFirstLine(size_t endLine);

		bool	_goodFirstLine(size_t endLine);
		size_t	_checkHttpVer(void);
		size_t	_checkStatusCode(void) const;
		size_t	_skipWS(void) const;
	};
	
} // namespace htpp
