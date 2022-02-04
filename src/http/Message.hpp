/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 09:03:17 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/04 12:30:06 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../shared/Buffer.hpp"
#include "CI_Less.hpp"
#include <map>

namespace	http
{
	class Message
	{
	public:
		typedef std::map<std::string, std::string, CI_Less>	header_m;

	protected:
		header_m			_header;
		ws::shared::Buffer	_body;
		size_t				_contentLength;

	public:
		Message(void);
		virtual ~Message(void);
		size_t				contentLength() const;
		void				setContentLength(size_t contentLength);
		header_m&			header(void);
		const header_m&		header(void) const;
		const std::string	header(const std::string field);
		const ws::shared::Buffer&	body(void) const;
		ws::shared::Buffer&	body(void);

	};
}