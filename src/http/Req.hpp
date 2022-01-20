/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Req.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:15:59 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/19 22:18:55 by vneirinc         ###   ########.fr       */
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

		Req(ws::shared::Buffer& buff);

		e_method			method(void) const;
		const std::string&	path(void) const;
		const std::string&	header(const std::string& field) const;
		const ws::shared::Buffer&	body(void) const;

		bool	finish(void) const;
		bool	update(ws::shared::Buffer& buff);

	private:
		e_method			_method;
		std::string			_path;
		header_m			_header;
		ws::shared::Buffer	_body;
		size_t				_contentLength;
		void				(Req::*_update)(ws::shared::Buffer& buff);	

		std::string		_getHeaderLine(const ws::shared::Buffer& buff, size_t& headerSize) const;
		size_t			_getMethod(std::string& line);
		void			_updateBody(ws::shared::Buffer& buff);
		void			_updateHeader(ws::shared::Buffer& buff);
		void			_endHeader(ws::shared::Buffer& buff);
		void			_getStartLine(std::string line);
		void			_insertHeaderField(std::string& line);
	};
} // namespace http