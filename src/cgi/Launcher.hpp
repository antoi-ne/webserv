#ifndef WS_CGI_LAUNCHER_HPP
# define WS_CGI_LAUNCHER_HPP

# include <string>
# include <cstdio>
# include <unistd.h>
# include <exception>
# include <map>
# include <algorithm>
# include <string>

# include "http/Req.hpp"
# include "http/Parser/ResParser.hpp"
# include "http/MsgUpdate.hpp"
# include "http/Res.hpp"
# include "shared/Log.hpp"
# include "conf/Config.hpp"

namespace ws
{
	namespace cgi
	{
		class Launcher
		{
		public:

			// cgi: full path in cgi_pass; script: full physical path of the script to execute
			Launcher(http::Req req, std::string host, in_port_t port, std::string cgi, std::string script);

			~Launcher();

			http::Res run();

		private:

			http::Req _req;
			conf::Location _loc;
			int _in[2];
			int _out[2];
			std::string _cgi;
			std::string _script;
			std::map<std::string, std::string> _env;

			shared::Buffer _subprocess(char * script, char **args, char **envp);
		};
	}
}

#endif
