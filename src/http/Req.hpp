/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:15:59 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/24 18:00:10 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../shared/Buffer.hpp"
#include "method.h"
#include "CI_Less.hpp"
#include <map>

namespace http
{
	class Req
	{
	public:
		typedef std::map<std::string, std::string, CI_Less>	header_m;

		Req(void);

		bool				hasHeader(void) const;
		e_method			method(void) const;
		const std::string&	path(void) const;
		header_m&			header(void);
		const header_m&		header(void) const;
		const std::string&	header(const std::string field);
		const ws::shared::Buffer&	body(void) const;

		bool	update(ws::shared::Buffer& buff);

	private:
		e_method			_method;
		std::string			_path;
		header_m			_header;
		ws::shared::Buffer	_buff;
		size_t				_contentLength;
		bool				_headerFinish;
		bool				(Req::*_check_line)(size_t endLine);

		size_t			_getMethod(std::string& line);
		bool			_checkStartLine(size_t endLine);
		bool			_skipStartCRLF(void);
		bool			_checkHeader(size_t endLine);
		bool			_getStartLine(size_t endLine);
		bool			_setHeader(size_t endLine);
		bool			_updateIfCRLF(void);
		bool			_isNotFinish(void) const;
		bool			_failed(void);
	};
} // namespace http