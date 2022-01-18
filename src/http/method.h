/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   method.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 10:18:12 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/18 11:05:16 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHOD_H
# define METHOD_H
# define HTTPVER "HTTP/1.1"
# define METHODS {"GET ", "POST ", "PUT ", "DELETE ", NULL}

enum e_method
{
	GET,
	POST,
	PUT,
	DELETE
};

#endif
