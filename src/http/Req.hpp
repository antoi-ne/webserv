/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancoulon <ancoulon@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 11:15:13 by vneirinc          #+#    #+#             */
/*   Updated: 2022/03/18 12:04:48 by ancoulon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Message.hpp"
#include "method.h"
#include <ctime>
#include <iostream>

namespace	http
{
	class Req : public Message
	{
	private:
		e_method			_method;
		std::string			_path;
		time_t				_createdTime;
	public:
		Req(void);
		e_method			method(void) const;
		const std::string&	path(void) const;

		void	setPath(const std::string& path);
		void	setMethod(e_method method);
		time_t	getCreatedTime(void) const;

		ws::shared::Buffer	getReq(void) const;
	private:
		bool	_hasBody(e_method method);
	};

}