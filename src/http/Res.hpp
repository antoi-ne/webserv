/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Res.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:13:56 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/18 18:23:06 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "res_header.h"
#include "../shared/Buffer.hpp"

namespace http
{
	class Res
	{
	private:
		unsigned int	_status;

	public:
		Res(void);

		void	sendRes(void) const;
	};
}