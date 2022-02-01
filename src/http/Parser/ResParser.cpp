/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResParser.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 13:42:39 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/01 14:12:59 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResParser.hpp"

namespace http
{
	ResParser::ResParser(http::Res& res)
	 : Parser(res), _res(res)
	{}

	bool	ResParser::_checkFirstLine(size_t endLine)
	{
		(void)endLine;
		(void)this->_res;
		return true;
	}
} // namespace htpp
