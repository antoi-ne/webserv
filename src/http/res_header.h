/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   res_header.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 14:21:56 by vneirinc          #+#    #+#             */
/*   Updated: 2022/01/26 09:05:39 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RES_HEADER
# define RES_HEADER
# include "method.h"
# define SERVER "webserv/0.1.0"
# define DATE_FORMAT "%a, %d %b %Y %H:%M:%S GMT"
# define STATUS200 "200 OK"
# define STATUS201 "201 CREATED"
# define STATUS400 "400 Bad Request"
# define STATUS403 "403 Forbidden"
# define STATUS404 "404 Not Found"
# define STATUS405 "405 Method Not Allowed"
# define STATUS413 "413 Request Entity Too Large"
# define STATUS444 "444 Wrong"
#endif
