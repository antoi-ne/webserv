/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Res.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancoulon <ancoulon@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:13:56 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/23 16:14:01 by ancoulon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "res_header.h"
#include "../shared/Buffer.hpp"
#include "../net/Connection.hpp"

namespace http
{

	class Res
	{
	private:
		std::string			_statusMsg;
		std::string			_contentType;
		ws::shared::Buffer	_body;

	public:
		Res(void);

		ws::shared::Buffer	get_res();

		void	setStatus(const std::string& statusMsg);
		void	setContentType(const std::string& contentType);

		ws::shared::Buffer&	body(void);

	};
}