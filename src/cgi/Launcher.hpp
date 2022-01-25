#ifndef WS_CGI_LAUNCHER_HPP
# define WS_CGI_LAUNCHER_HPP

# include <string>
# include <cstdio>
# include <unistd.h>
# include <exception>

# include "http/Req.hpp"
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

			Launcher(http::Req req, conf::Server srv, conf::Location loc);

			~Launcher();

			http::Res run();

		private:

			http::Req _req;
			conf::Location _loc;
			int _in[2];
			int _out[2];
			std::string _cgi;
			std::string _script;

			bool _subprocess();
		};
	}
}

#endif
