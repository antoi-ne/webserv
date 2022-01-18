/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Res.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:15:36 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/18 18:26:45 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Res.hpp"

namespace http
{

	Res::Res(void)
	 : _status(200)
	{}

	void	Res::sendRes(void) const
	{
		std::string	buff = (HTTPVER);

		buff += std::to_string(this->_status);
		buff += " OK\r\nServer: webserv/0.1.0\r\nDate: ";
	}
}