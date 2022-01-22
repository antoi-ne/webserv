#include "Controller.hpp"

namespace ws
{
	namespace core
	{

		Controller::Controller(const conf::Config& config)
			: _config(config), _srv(), _pool(), _req_cache(), _res_cache(), _router(config)
		{
			conf::server_map::const_iterator it;
			for (it = config.servers.begin(); it != config.servers.end(); it++)
				this->_srv.push_back(net::Server(it->first.first, it->first.second));
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
			shared::Buffer buff;


			ctxs = this->_pool.probe();
			for (it = ctxs.begin(); it != ctxs.end(); it++)
			{
				if (it->rread) // new request fragment received
				{
					buff = it->con.recv(4096);
					shared::Log::info("received data from " + it->con.get_address());
					if (this->_req_cache[it->con].update(buff) == false)
					{
						shared::Log::info(this->_req_cache[it->con].body().to_string());
						shared::Log::info("completed request");
						std::cout << this->_req_cache[it->con].method() << std::endl;
						if (this->_req_cache[it->con].method() == UNDEF)
							it->con.send(std::string("HTTP/1.1 400 Bad Request\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\nBad Request\r\n"));
						else
						{
							const http::Res res = this->_router.process(
								this->_req_cache[it->con],
								std::make_pair(it->srv.get_host(),
								it->srv.get_port())
							);
							this->_res_cache[it->con].push_back(res);
						}
					this->_req_cache.erase(it->con);
					}
					else
						shared::Log::info("request not complete");
				}

				if (it->rwrite && !this->_res_cache[it->con].empty()) // ready to receive response if any response is in the cache
				{
					this->_res_cache[it->con].front().sendRes(it->con);
					this->_res_cache[it->con].clear();
				}
			}
		}
	}
}
