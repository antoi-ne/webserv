/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Router.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vneirinc <vneirinc@students.s19.be>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 17:40:33 by vneirinc          #+#    #+#             */
/*   Updated: 2022/03/24 09:34:42 by vneirinc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Router.hpp"
#include "index_of.h"
#include "../cgi/Launcher.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

namespace ws
{
	namespace core
	{
		bool	_hasBody(e_method method) { return method == POST || method == PUT; }

		Router::Router(const conf::Config& conf) : _config(conf), _mime()
    	{
			std::string mime_type[] = MIME_TYPE;
			std::string mime_ext[] = MIME_EXT;
			size_t		i = 0;

			while (!(mime_ext[i].empty()))
			{
				this->_initMIME(mime_type[i], mime_ext[i]);
				++i;
			}
	    }

		void	Router::_initMIME(const std::string mime_type, const std::string mime_ext)
		{
			size_t		i = 0;
			size_t		start_ext = 0;
			std::vector<std::string>	tmp;
			
			while (i < mime_ext.size())
			{
				if (mime_ext[i] == ';')
				{
					tmp.push_back(mime_ext.substr(start_ext, i));
					if (mime_ext[i] == ' ')
						++i;
					start_ext = i + 2;
				}
				++i;
			}
			tmp.push_back(mime_ext.substr(start_ext, i));
			this->_mime.push_back(make_pair(tmp, mime_type));
		}

		void
		Router::_process(
			const ReqU& request,
			http::Res& response,
			const conf::ServConfig& mainConf,
			const conf::host_port& host) const
		{
			std::pair<const char *, uint16_t>	err;
			std::string							ext;

			if (!request.error())
				err = this->_processServ(request, response, mainConf, host);
			else
				err = std::make_pair(STATUS400, 400);
			if (err.second)
				return this->_setError(response, mainConf, err.first, err.second);
		}

		http::Res
		Router::process(const ReqU& request, const conf::host_port& host) const
		{
			http::Res					response;
			const conf::Server*			serv;
			const conf::Location*		loc;
			
			if (request.header("connection") == "close")
				response.header()["connection"] = "close";
			serv = this->_getServ(request.header("host"), host);
			if ((loc = this->_getLocation(request.path(), *serv)))
				this->_process(request, response, *loc, host);
			else
				this->_process(request, response, *serv, host);
			return response;
		}

		const conf::Server*
		Router::_getServ(const std::string& headerHost, const conf::host_port& host) const
		{
			conf::server_map::const_iterator it = this->_config.servers.find(host);

			return _getServerName(headerHost, it->second);
		}

		const conf::Server*
		Router::_getServerName(const std::string& host, const serv_lst& servLst) const
		{
			if (servLst.size() > 1 && !host.empty())
			{
				CI_Less	ci;

				for (serv_lst::const_iterator it = servLst.begin(); it != servLst.end(); ++it)
					for (std::vector<std::string>::const_iterator iter = it->server_names.begin(); iter != it->server_names.end(); ++iter)
						if (!ci(*iter, host) && !ci(host, *iter))
							return it.base();
			}
			return servLst.begin().base();
		}

		const conf::Location*
		Router::_getLocation(const std::string& uri, const conf::Server& serv) const
		{
			for (conf::location_v::const_iterator it = serv.locations.begin();
			it != serv.locations.end(); ++it)
				if (uri.find(it->route) == 0)
					return it.base();
			return NULL;
		}

		void
		Router::_setError(
			http::Res& response,
			const conf::ServConfig& serv,
			const char* str,
			const uint16_t code) const
		{
			const std::string*	errorPage;
			std::string			ext;

			response.setStatus(str);
			errorPage = this->_findErrorPage(serv, code);
			if (errorPage)
			{
				this->_getFile(response.body(), *errorPage);
				ext = this->_getExt(*errorPage);
			}
			if (code == 400)
				response.header()["connection"] = "close";
			this->_getMIME(response, ext);
		}

		const std::string*
		Router::_findErrorPage(const conf::ServConfig& serv, const uint16_t code) const
		{
			conf::ErrorPages::const_iterator it = serv.error_pages.find(code);

			if (it == serv.error_pages.end())
				return NULL;
			return &it->second;
		}

		std::pair<const char *, uint16_t>
		Router::_processServ(
			const http::Req& request,
			http::Res& response,
			const conf::ServConfig& mainConf,
			const conf::host_port& host) const
		{
			if (std::find(mainConf.accepted_methods.begin(), mainConf.accepted_methods.end(), request.method())
				== mainConf.accepted_methods.end())
				return std::make_pair(STATUS405, 405);
			if (!this->_checkMaxBodySize(mainConf, request.body().size()))
				return std::make_pair(STATUS413, 413);
			if (_hasBody(request.method()) && mainConf.upload_path.size())
			{
				std::pair<const char *, uint16_t>	err = this->_upload(request, mainConf);
				if (err.second)
					return err;
			}
			else if (!mainConf.return_path.size())
				return this->_renderPage(request, response, mainConf, host);
			if (mainConf.return_path.size())
			{
				response.setStatus(mainConf.return_code);
				response.header().insert(std::make_pair("Location", mainConf.return_path));
			}
			return std::make_pair("", 0);
		}

		std::pair<const char *, uint16_t>	
		Router::_upload(
			const http::Req& request,
			const conf::ServConfig& mainConf) const
		{
			std::pair<std::string, ws::shared::Buffer>	content;
			std::string									boundary;
			
			boundary = "--" + this->_getBoundary(request.header("content-type"));
			content = this->_parseFormData(boundary, request.body());
			if (!content.first.size())
				return std::make_pair(STATUS400, 400);
			else if (!this->_writeFile(mainConf.upload_path + content.first, content.second))
				return std::make_pair(STATUS413, 403);
			return std::make_pair("", 0);
		}

		std::string
		Router::_getBoundary(std::string headerField) const
		{
			size_t pos = headerField.find("boundary");

			if (pos == std::string::npos)
				return "";
			return headerField.c_str() + pos + 9;
			
		}

		size_t
		_filename(std::string& filename, const ws::shared::Buffer& body, size_t start)
		{
			size_t	pos = body.find(start, "filename=\"");
			size_t i = pos;

			if (pos != std::string::npos)
			{
				pos += 10;
				i = pos;
				while (body[i] != '"')
					++i;
				filename = "/" + std::string(body.get_ptr() + pos, i - pos);
				if ((i = body.find(i, "\n")) != std::string::npos)
					if ((i = body.find(++i, "\n")) != std::string::npos)
						if ((i = body.find(++i, "\n")))
							++i;
			}
			return i;
		}

		std::pair<std::string, ws::shared::Buffer>
		Router::_parseFormData(
			const std::string& boundary,
			const ws::shared::Buffer& body) const
		{
			std::string			filename;
			ws::shared::Buffer	content;
			size_t				pos;

			if (boundary.size() > 2)
			{
				if ((pos = body.find(boundary)) != std::string::npos)
				{
					pos += boundary.size();
					if ((pos = _filename(filename, body, pos)) != std::string::npos)
					{
						size_t endData = body.find_last_of_from(boundary.c_str(), body.size() - 1);
						if (endData != std::string::npos)
						{
							if ((endData = body.find_last_of_from("\n", endData)) != std::string::npos)
							{
								--endData;
								if (body[endData] == '\r')
									--endData;
								content.join(body.get_ptr() + pos, endData - pos);
							}
						}
					}
				}
			}
			return std::make_pair(filename, content);
		}

		bool	
		Router::_writeFile(const std::string& path, const shared::Buffer& buff) const
		{
			std::ofstream	file(path);

			if (file)
			{
				file.write(buff.get_ptr(), buff.size());
				file.close();
				return true;
			}
			return false;
		}

		std::pair<const char *, uint16_t>
		Router::_renderPage(
			const http::Req& request,
			http::Res& response,
			const conf::ServConfig& mainConf,
			const conf::host_port& host) const
		{
			std::string				path;
			std::string				ext;
			shared::Buffer			body;

			path = _getLocalPath(request.method() ,request.path(), mainConf);
			if (path.empty())
				return std::make_pair(STATUS404, 404);
			ext = this->_getExt(path);
			if (ext.size())
			{
				if (mainConf.cgi_ext.size() && mainConf.cgi_ext == ext)
				{
					try {
						response = cgi::Launcher(request, host.first, host.second, mainConf.cgi_script, path).run();
					} catch (...) {
						return std::make_pair(STATUS500, 500);
					}
					return std::make_pair("", 0);
				}
				else
					this->_getMIME(response, path.back() == '/' ? "html" : ext);
			}
			if (request.method() == DELETE)
			{
				if (!this->_delete(path))
					return std::make_pair(STATUS500, 500);
			}
 			else if (!this->_getBody(body, path, request.path()))
				return std::make_pair(STATUS500, 500);
			response.body().join(body);
			return std::make_pair("", 0);
		}

		bool
		Router::_deleteDir(const std::string& path, DIR* dirp) const
		{
    		struct dirent*	dp;

    		while ((dp = readdir(dirp)) != NULL)
				if (dp->d_name[0] != '.' || (dp->d_name[1] != '\0' && dp->d_name[1] != '.'))
					if (!this->_delete(path + "/" + dp->d_name))
						return false;
			return true;
		}

		bool
		Router::_delete(const std::string& path) const
		{
			bool		ret = true;
			struct stat	info;

			if (stat(path.c_str(), &info) == 0)
			{
				if (S_ISDIR(info.st_mode))
				{
					DIR* dirp = opendir(path.c_str());
					if (!dirp || !this->_deleteDir(path, dirp))
						ret = false;
					closedir(dirp);
				}
				if (remove(path.c_str()) == -1)
					ret = false;
			}
			return ret;
		}

		std::string
		Router::_getExt(const std::string& path) const
		{
			size_t	ext_start = path.find_last_of('.');

			if (ext_start == std::string::npos)
				return "";
			return path.substr(ext_start + 1);
		}

		std::string
		Router::_getLocalPath(
			const e_method	method,
			const std::string& uri,
			const conf::ServConfig& serv) const
		{
			std::string	path;
			struct stat	info;

			path = serv.root;
			path += (uri.c_str() + serv.route.size());
			if (stat(path.c_str(), &info) != 0)
				return std::string();
			if (S_ISDIR(info.st_mode))
			{
				if (path.back() != '/')
					path.push_back('/');
				if (!serv.autoindex && method != DELETE)
				{
					path.append(serv.index);
					if (stat(path.c_str(), &info) != 0)
						return std::string();
				}
			}
			return path;
		}

		void
		Router::_getMIME(http::Res& res, const std::string& ext) const
		{
			mime_vec::const_iterator	it;

			it = this->_mime.begin();
			for (; it != this->_mime.end(); ++it)
			{
				std::vector<std::string>::const_iterator	ext_it = it->first.begin();

				for (;ext_it != it->first.end(); ++ext_it)
				{
					if (*ext_it == ext)
					{
						res.header()["Content-Type"] = it->second;
						return ;
					}
				}
			}
		}

		bool
		Router::_getBody(shared::Buffer& body, const std::string& path, const std::string& uri) const
		{
			if (path.back() == '/') // autoindex on case
			{
    			DIR* dirp = opendir(path.c_str());
				if (!dirp)
					return false;
				body = this->_getAutoIndexPage(dirp, uri);
				closedir(dirp);
			}
			else
				return this->_getFile(body, path);
			return true;
		}

		shared::Buffer
		Router::_getAutoIndexPage(DIR* dirp ,const std::string& uri) const
		{
			std::vector<struct dirent>	dirList;
			shared::Buffer	buff;
			char			_buff[512];
			size_t			size;

			size = sprintf(_buff, INDEX_OF1, uri.c_str(), uri.c_str());
			buff.join(_buff, size);

			dirList = this->_getDirList(dirp);
			for (std::vector<struct dirent>::const_iterator it = ++dirList.begin(); it != dirList.end(); ++it)
			{
				if (it->d_type == DT_DIR)
					size = sprintf(_buff, DIR_TEMP, uri.c_str(), it->d_name, it->d_name);
				else
					size = sprintf(_buff, FILE_TEMP, uri.c_str(), it->d_name, it->d_name);
				buff.join(_buff, size);
			}

			buff.join(INDEX_OF2, 25);
			return buff;
		}

		const std::vector<struct dirent>
		Router::_getDirList(DIR* dirp) const
		{
			std::vector<struct dirent>	v;
    		struct dirent*				dp;

    		while ((dp = readdir(dirp)) != NULL)
				v.push_back(*dp);
			return v;
		}

		bool
		Router::_getFile(shared::Buffer& body, const std::string& path) const
		{
			std::ifstream	file(path);
			char			buff[2048];
			size_t			len;

			if (!file)
				return false;
			while (!file.eof())
			{
				file.read(buff, 2048);
				len = file.gcount();
				body.join(buff, len);
			}
			file.close();
			return true;
		}

		bool
		Router::_checkMaxBodySize(const conf::ServConfig& serv, size_t bodySize) const
		{
			if (serv.max_body_size != -1)
				return bodySize <= static_cast<size_t>(serv.max_body_size);
			return true;
		}
	}
}
