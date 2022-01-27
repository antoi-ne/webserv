#include "Launcher.hpp"

namespace ws
{
	namespace cgi
	{
		http::Res Launcher::run(http::Req req, std::string script, std::string file)
		{
			http::Res res;
			(void)req;
			(void)script;
			(void)file;

			// [...]

			return res;
		}
	}
}