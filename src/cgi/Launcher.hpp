#ifndef WS_CGI_LAUNCHER_HPP
# define WS_CGI_LAUNCHER_HPP

# include <string>
# include <cstdio>
# include <unistd.h>
# include <exception>

# include "http/Req.hpp"
# include "http/Res.hpp"
# include "shared/Log.hpp"

namespace ws
{
	namespace cgi
	{
		class Launcher
		{
		public:

			Launcher(http::Req req);

			~Launcher();

			http::Res run();

		private:

			http::Req _req;
			int _in[2];
			int _out[2];

			bool _subprocess();
		};
	}
}

#endif
