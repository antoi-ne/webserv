/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Status.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:31:34 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/18 14:35:38 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
namespace http
{
	class Status
	{
	private:
		unsigned int	_code;
		std::string		_msg;
	
	public:
		Status(void);

		void	setCode(unsigned int code);

		unsigned int	code(void) const;
		const std::string&	msg(void) const;
	};
}