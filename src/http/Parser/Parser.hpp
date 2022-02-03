/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 10:56:33 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/03 09:32:48 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../../shared/Buffer.hpp"
#include "../Message.hpp"
#include <iostream>

namespace	http
{
	class Parser
	{
	private:
		http::Message&		_msg;

	protected:
		ws::shared::Buffer	_buff;
		bool				_headerFinish;
		bool 				(Parser::*_fUpdate)(size_t);
		
	public:
		Parser(http::Message& msg);
		Parser(http::Message& msg, bool (Parser::*fUpdate)(size_t));

		void		chillCheck(const ws::shared::Buffer& buff);
		bool		update(const ws::shared::Buffer& buff);
		bool		headerFinish(void) const;
		bool		checkHeader(size_t endLine);
		virtual	bool	checkFirstLine(size_t endLine) = 0;

	protected:
		bool				_acceptedChar(const char c) const;

	private:
		bool				_unchunkedBody(void);
		bool				_chunkedSize(size_t endLine, size_t& chunkSize) const;
		bool				_chunkedContent(size_t& chunkSize);
		bool				_setHeader(size_t endLine);
		void				_endHeader(size_t endLine);
		bool				_isNotFinish(void);
		bool				_updateIfCRLF(void);
		void				_chillIfCRLF(void);
		http::Message&		_getMsg(void);
	};
}