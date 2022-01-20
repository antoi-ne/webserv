#include "Controller.hpp"

namespace ws
{
	namespace core
	{

		Controller::Controller(conf::Config config)
			: _config(config), _srv(), _pool(), _req_cache(), _res_cache()
		{
			std::vector<conf::Server>::iterator it;
			std::list<net::Server>::iterator itn;
			net::Server srv;
			bool duplicate = false;

			for (it = this->_config.servers.begin(); it != this->_config.servers.end(); it++)
			{
				for (itn = this->_srv.begin(); itn != this->_srv.end(); itn++)
				{
					if (itn->get_host() == it->host && itn->get_port() == it->port)
					{
						duplicate = true;
						break;
					}
				}
				if (duplicate)
					break;
				srv = net::Server(it->host, it->port, 20);
				this->_srv.push_back(srv);
			}
		}

		Controller::~Controller()
		{}

		void Controller::start()
		{
			std::list<net::Server>::iterator it;

			for (it = this->_srv.begin(); it != this->_srv.end(); it++)
				it->listen();

			this->_req_cache.clear();
			this->_res_cache.clear();

			this->_pool = net::Pool(this->_srv);

			for(;;)
				this->_loop();
		}

		void Controller::_loop()
		{
			std::list<net::Ctx> ctxs;
			std::list<net::Ctx>::iterator it;

			ctxs = this->_pool.probe();
			for (it = ctxs.begin(); it != ctxs.end(); it++)
			{
				if (it->rread)
				{
					shared::Buffer buff = it->con.recv(4096);
					// shared::Log::info(buff.to_string());
					shared::Log::info("received data");
					if (this->_req_cache[it->con].update(buff) == false)
					{
						shared::Log::info(this->_req_cache[it->con].body().get_ptr());
						shared::Log::info("completed request");
						std::cout << this->_req_cache[it->con].method() << std::endl;
						if (this->_req_cache[it->con].method() == UNDEF)
						{
							it->con.send(std::string("HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello WORLD!"));
						}
						else
							it->con.send(std::string("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!"));
						this->_req_cache.erase(it->con);
						this->_pool.close_con(it->con);
					}
					else
						shared::Log::info("request not complete");
				}
			}
		}
	}
}
