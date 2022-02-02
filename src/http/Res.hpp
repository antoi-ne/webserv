/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Res.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:13:56 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/01 16:55:47 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Message.hpp"
#include "res_header.h"

namespace http
{
	class Res : public Message
	{
	private:
		std::string			_statusMsg;

	public:
		Res(void);

		ws::shared::Buffer	get_res();

		void	setStatus(const std::string& statusMsg);
	private:
		std::string	_getTime(void) const;

	};
}