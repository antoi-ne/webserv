/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CI_Less.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 09:58:41 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/21 10:00:05 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <functional>
#include <algorithm>
#include <string>

struct CI_Less : std::binary_function<std::string, std::string, bool>
{
	struct nocase_compare :
		public std::binary_function<unsigned char,unsigned char,bool> 
	{
		bool operator() (const unsigned char& c1, const unsigned char& c2) const
		{ return tolower (c1) < tolower (c2); }
	};

	bool operator() (const std::string & s1, const std::string & s2) const
	{
		return std::lexicographical_compare(s1.begin(), s1.end(),
				s2.begin(), s2.end(), nocase_compare());
	}
};