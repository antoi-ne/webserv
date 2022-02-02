#include "Launcher.hpp"

namespace ws
{
	namespace cgi
	{
		Launcher::Launcher(http::Req req, std::string host, in_port_t port, std::string cgi, std::string script)
			: _req(req), _cgi(cgi), _script(script), _env()
		{
			if (::pipe(this->_in))
				throw std::runtime_error("syscall pipe failed");
			if (::pipe(this->_out))
				throw std::runtime_error("syscall pipe failed");

			this->_env["SERVER_SOFTWARE"] = "webserv/0.1";
			this->_env["SERVER_NAME"] = host;
			this->_env["GATEWAY_INTERFACE"] = "CGI/1.1";
			this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
			this->_env["SERVER_PORT"] = std::to_string(port);
			this->_env["PATH_INFO"] = req.path();
			this->_env["PATH_TRANSLATED"] = script;
			this->_env["SCRIPT_FILENAME"] = script;
			this->_env["CONTENT_TYPE"] = req.header("Content-Type");
			this->_env["CONTENT_LENGTH"] = req.header("Content-Length");

			switch (req.method())
			{
				case GET:
					this->_env["REQUEST_METHOD"] = "GET";
					break;
				case POST:
					this->_env["REQUEST_METHOD"] = "POST";
					break;
				case PUT:
					this->_env["REQUEST_METHOD"] = "PUT";
					break;
				case DELETE:
					this->_env["REQUEST_METHOD"] = "DELETE";
					break;
				default:
					throw std::runtime_error("unknown method");
			}

			for (http::Req::header_m::iterator it = req.header().begin(); it != req.header().end(); it++)
			{
				std::string key("HTTP_");
				key.append(it->first);
				for (size_t i = 5; i < key.length(); i++)
				{
					if (key[i] == '-')
						key[i] = '_';
					else
						key[i] = ::toupper(key[i]);
				}
				this->_env[key] = it->second;
			}
		}

		Launcher::~Launcher()
		{}

		Launcher::Response Launcher::run()
		{
			Response res;
			char ** envp;
			char ** args;
			char *script;
			size_t i = 0;
			shared::Buffer buff;

			envp = new char*[this->_env.size() + 1]();
			for (std::map<std::string, std::string>::iterator it = this->_env.begin(); it != this->_env.end(); it++)
			{
				envp[i] = strdup((std::string(it->first) + "=" + std::string(it->second)).c_str());
				i++;
			}
			envp[this->_env.size()] = NULL;

			args = new char*[3]();
			if ((args[0] = strdup(this->_cgi.c_str())) == NULL)
				throw std::runtime_error("strdup failed");
			if ((args[1] = strdup(this->_script.c_str())) == NULL)
				throw std::runtime_error("strdup failed");
			args[2] = NULL;

			if ((script = strdup(this->_cgi.c_str())) == NULL)
				throw std::runtime_error("strdup failed");

			shared::Buffer raw_req = this->_req.body();
			raw_req.resetCursor();

			if (::write(this->_in[1], raw_req.get_ptr(), raw_req.size()) < (ssize_t)raw_req.size())
				throw std::runtime_error("syscall write failed");

			::close(this->_in[1]);

			buff = this->_subprocess(script, args, envp);

			if (res.update(buff) == true || res.error() == true)
			{
				throw std::runtime_error("invalid response from cgi");
			}
			return res;
		}

		shared::Buffer Launcher::_subprocess(char *script, char **args, char **envp)
		{
			pid_t pid;
			int ret;
			shared::Buffer buffer;

			std::cout << "script: " << script << std::endl;

			pid = ::fork();
			if (pid < 0)
				throw std::runtime_error("syscall fork failed");
			else if (pid == 0)
			{
				::close(this->_out[0]);
				if (dup2(this->_in[0], STDIN_FILENO) < 0)
					shared::Log::fatal("syscall dup2 failed");
				if (dup2(this->_out[1], STDOUT_FILENO) < 0)
					shared::Log::fatal("syscall dup2 failed");


				if (::execve(script, args, envp) != 0)
					shared::Log::fatal("execve failed");
			}
			else
			{
				::close(this->_out[1]);
				::close(this->_in[0]);
				char buff[2048 + 1] = {0};
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

				::close(this->_out[0]);
			}
			return buffer;
		}

	}
}
