/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:15:59 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/18 18:22:26 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../shared/Buffer.hpp"
#include "method.h"
#include <map>

namespace http
{
	class Req
	{
	public:
		typedef std::map<std::string, std::string>	header_m;

		Req(ws::shared::Buffer buff);

		e_method			method(void) const;
		const std::string&	path(void) const;
		const std::string&	header(const std::string& field) const;
		const ws::shared::Buffer&	body(void) const;

	private:
		e_method			_method;
		std::string			_path;
		header_m			_header;
		ws::shared::Buffer	_body;

		std::string		_getNextHttpLine(const ws::shared::Buffer& buff, size_t& headerSize) const;
		size_t			_getMethod(std::string& line);
		void			_getStartLine(std::string line);
		void			_getHeader(const ws::shared::Buffer& buff, size_t& headerSize);
		void			_insertHeaderField(std::string& line);
	};
} // namespace http