/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MsgUpdate.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 13:56:59 by vneirinc          #+#    #+#             */
/*   Updated: 2022/02/03 09:26:18 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../shared/Buffer.hpp"

namespace http
{
	template <class Msg, class Parser>
	class MsgUpdate : public Msg
	{
	private:
		Parser	_parser;
	public:
		MsgUpdate(void) : _parser(*this) {}

		bool	error(void) const
		{ return !this->_parser.headerFinish(); }

		bool	update(const ws::shared::Buffer& buff)
		{ return this->_parser.update(buff); }

		void	chillCheck(const ws::shared::Buffer& buff)
		{ this->_parser.chillCheck(buff); }
	};

} // namespace http