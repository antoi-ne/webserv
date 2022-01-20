/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:15:59 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/20 14:13:39 by vneirinc         ###   ########.fr       */
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

		Req(void);

		e_method			method(void) const;
		const std::string&	path(void) const;
		const std::string&	header(const std::string& field);
		const ws::shared::Buffer&	body(void) const;

		bool	update(ws::shared::Buffer& buff);

	private:
		e_method			_method;
		std::string			_path;
		header_m			_header;
		ws::shared::Buffer	_buff;
		size_t				_contentLength;
		bool				_hasHeader;

		bool			_checkHeader(void);
		std::string		_getNextHeaderLine(void);
		size_t			_getMethod(std::string& line);
		bool			_updateBody(ws::shared::Buffer& buff);
		bool			_updateHeader(ws::shared::Buffer& buff);
		bool			_endHeader(void);
		void			_getStartLine(void);
		void			_insertHeaderField(std::string& line);
		void			_setHeader(void);
	};
} // namespace http