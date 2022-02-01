/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 13:43:01 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/01 15:53:59 by vneirinc         ###   ########.fr       */
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

		bool	_checkHttpVer(void);
		bool	_checkStatusCode(void) const;
		void	_skipWS(void);
	};
	
} // namespace htpp
