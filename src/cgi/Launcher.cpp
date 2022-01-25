#include "Launcher.hpp"

namespace ws
{
	namespace cgi
	{
		Launcher::Launcher(http::Req req, conf::Server srv, conf::Location loc)
			: _req(req), _loc(loc)
		{
			if (::pipe(this->_in))
				throw std::runtime_error("syscall fork failed");
			if (::pipe(this->_out))
				throw std::runtime_error("syscall fork failed");
			
			if (loc.cgi_path[0] == '/')
				this->_cgi = loc.cgi_path;
			else
			{
				char * wd;
				if (wd = ::getcwd(NULL, 0))
					throw std::runtime_error("syscall getcwd failed");
				this->_cgi = std::string(wd) + loc.cgi_path;
			}

			if (loc.root.size() == 0)
				this->_script = srv.root + req.path();
			else
				this->_script = loc.root + req.path();
		}

		Launcher::~Launcher()
		{}

		http::Res Launcher::run()
		{
			http::Res res;

			// [...]

			return res;
		}

		bool Launcher::_subprocess()
		{
			pid_t pid;
			int ret;


			pid = ::fork();
			if (pid < 0)
				throw std::runtime_error("syscall fork failed");
			else if (pid == 0)
			{
				if (dup2(this->_in[0], STDIN_FILENO) != 0)
					shared::Log::fatal("syscall dup2 failed");
				if (dup2(this->_out[1], STDOUT_FILENO) != 0)
					shared::Log::fatal("syscall dup2 failed");

				if (::execve(NULL, NULL, NULL))
					shared::Log::fatal("execve failed");
			}
			else
			{
				char buff[2048 + 1] = {0};
				shared::Buffer buffer;

				waitpid(pid, &ret, 0);
				ret = WEXITSTATUS(ret);

				if (ret != 0)
					throw std::runtime_error("cgi script failed");

				while (::read(this->_out[0], buff, 2048) > 0)
				{
					buffer.join(std::string(buff));
					std::memset(buff, 0, 2049);
				}
				if (ret < 0)
					throw std::runtime_error("syscall read failed");
			}
			return true;
		}

	}
}
