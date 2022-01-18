/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:33:42 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/18 14:36:25 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Status.hpp"

namespace http
{

	unsigned int	Status::code(void) const { return this->_code; }

	const std::string&	Status::msg(void) const { return this->_msg; }

	Status::Status(void)
	 : _code(200), _msg("OK")
	{}

	void	Status::setCode(unsigned int code)
	{
		this->_code = code;
	}

}