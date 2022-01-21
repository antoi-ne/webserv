#ifndef WS_CGI_LAUNCHER_HPP
# define WS_CGI_LAUNCHER_HPP

# include <string>

# include "http/Req.hpp"
# include "http/Res.hpp"

namespace ws
{
	namespace cgi
	{
		class Launcher
		{
		public:

			static http::Res run(http::Req, std::string script, std::string file);

		private:
		};
	}
}

#endif
