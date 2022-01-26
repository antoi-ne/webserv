/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancoulon <ancoulon@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 10:18:12 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/26 14:49:35 by ancoulon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHOD_H
# define METHOD_H
# define HTTPVER "HTTP/1.1"
# define METHODS {"GET ", "POST ", "PUT", "DELETE "}
# define N_METHOD 4

enum e_method
{
	UNDEF = -1,
	GET,
	POST,
	PUT,
	DELETE
};

#endif
