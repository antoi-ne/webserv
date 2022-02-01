/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResParser.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 13:43:01 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/01 14:01:34 by vneirinc         ###   ########.fr       */
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
		http::Res&	_res;
		
	public:
		ResParser(http::Res& res);
	private:
		virtual bool	_checkFirstLine(size_t endLine);
	};
	
} // namespace htpp
